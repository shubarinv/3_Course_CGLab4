//
// Simple .obj viewer(vertex only)
//
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#if defined(ENABLE_ZLIB)
  #include <zlib.h>
#endif

#if defined(ENABLE_ZSTD)
  #include <zstd.h>
#endif

#include <GLFW/glfw3.h>

#define TINYOBJ_LOADER_OPT_IMPLEMENTATION
#include "tinyobj_loader_opt.h"
class ObjLoader {

  struct loadedOBJ {
	tinyobj_opt::attrib_t attrib;
	std::vector<tinyobj_opt::shape_t> shapes;
	std::vector<tinyobj_opt::material_t> materials;
  };
  void CalcNormal(float N[3], const float v0[3], const float v1[3], const float v2[3]) {
	float v10[3];
	v10[0] = v1[0] - v0[0];
	v10[1] = v1[1] - v0[1];
	v10[2] = v1[2] - v0[2];

	float v20[3];
	v20[0] = v2[0] - v0[0];
	v20[1] = v2[1] - v0[1];
	v20[2] = v2[2] - v0[2];

	N[0] = v20[1] * v10[2] - v20[2] * v10[1];
	N[1] = v20[2] * v10[0] - v20[0] * v10[2];
	N[2] = v20[0] * v10[1] - v20[1] * v10[0];

	float len2 = N[0] * N[0] + N[1] * N[1] + N[2] * N[2];
	if (len2 > 0.0f) {
	  float len = sqrtf(len2);

	  N[0] /= len;
	  N[1] /= len;
	}
  }

  const char* mmap_file(size_t* len, const char* filename) {
	(*len) = 0;
#ifdef _WIN32
	HANDLE file = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	assert(file != INVALID_HANDLE_VALUE);

	HANDLE fileMapping = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
	assert(fileMapping != INVALID_HANDLE_VALUE);

	LPVOID fileMapView = MapViewOfFile(fileMapping, FILE_MAP_READ, 0, 0, 0);
	auto fileMapViewChar = (const char*)fileMapView;
	assert(fileMapView != NULL);

	LARGE_INTEGER fileSize;
	fileSize.QuadPart = 0;
	GetFileSizeEx(file, &fileSize);

	(*len) = static_cast<size_t>(fileSize.QuadPart);
	return fileMapViewChar;

#else

	FILE* f = fopen(filename, "rb");
	if (!f) {
	  fprintf(stderr, "Failed to open file : %s\n", filename);
	  return nullptr;
	}
	fseek(f, 0, SEEK_END);
	long fileSize = ftell(f);
	fclose(f);

	if (fileSize < 16) {
	  fprintf(stderr, "Empty or invalid .obj : %s\n", filename);
	  return nullptr;
	}

	struct stat sb {};
	char* p;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1) {
	  perror("open");
	  return nullptr;
	}

	if (fstat(fd, &sb) == -1) {
	  perror("fstat");
	  return nullptr;
	}

	if (!S_ISREG(sb.st_mode)) {
	  fprintf(stderr, "%s is not a file\n", filename);
	  return nullptr;
	}

	p = (char*)mmap(nullptr, fileSize, PROT_READ, MAP_SHARED, fd, 0);

	if (p == MAP_FAILED) {
	  perror("mmap");
	  return nullptr;
	}

	if (close(fd) == -1) {
	  perror("close");
	  return nullptr;
	}

	(*len) = fileSize;

	return p;

#endif
  }

  bool gz_load([[maybe_unused]] std::vector<char>* buf, [[maybe_unused]] const char* filename) {
#ifdef ENABLE_ZLIB
	gzFile file;
	file = gzopen(filename, "r");
	if (!file) {
	  fprintf(stderr, "gzopen of '%s' failed: %s.\n", filename,
			  strerror(errno));
	  exit(EXIT_FAILURE);
	  return false;
	}
	while (1) {
	  int err;
	  int bytes_read;
	  unsigned char buffer[1024];
	  bytes_read = gzread(file, buffer, 1024);
	  buf->insert(buf->end(), buffer, buffer + 1024);
	  //printf ("%s", buffer);
	  if (bytes_read < 1024) {
		if (gzeof(file)) {
		  break;
		} else {
		  const char* error_string;
		  error_string = gzerror(file, &err);
		  if (err) {
			fprintf(stderr, "Error: %s.\n", error_string);
			exit(EXIT_FAILURE);
			return false;
		  }
		}
	  }
	}
	gzclose(file);
	return true;
#else
	return false;
#endif
  }

#ifdef ENABLE_ZSTD
  static off_t fsize_X(const char* filename) {
	struct stat st;
	if (stat(filename, &st) == 0) return st.st_size;
	/* error */
	printf("stat: %s : %s \n", filename, strerror(errno));
	exit(1);
  }

  static FILE* fopen_X(const char* filename, const char* instruction) {
	FILE* const inFile = fopen(filename, instruction);
	if (inFile) return inFile;
	/* error */
	printf("fopen: %s : %s \n", filename, strerror(errno));
	exit(2);
  }

  static void* malloc_X(size_t size) {
	void* const buff = malloc(size);
	if (buff) return buff;
	/* error */
	printf("malloc: %s \n", strerror(errno));
	exit(3);
  }
#endif

  bool zstd_load([[maybe_unused]] [[maybe_unused]] std::vector<char>* buf, [[maybe_unused]] const char* filename) {
#ifdef ENABLE_ZSTD
	off_t const buffSize = fsize_X(filename);
	FILE* const inFile = fopen_X(filename, "rb");
	void* const buffer = malloc_X(buffSize);
	size_t const readSize = fread(buffer, 1, buffSize, inFile);
	if (readSize != (size_t)buffSize) {
	  printf("fread: %s : %s \n", filename, strerror(errno));
	  exit(4);
	}
	fclose(inFile);

	unsigned long long const rSize = ZSTD_getDecompressedSize(buffer, buffSize);
	if (rSize == 0) {
	  printf("%s : original size unknown \n", filename);
	  exit(5);
	}

	buf->resize(rSize);

	size_t const dSize = ZSTD_decompress(buf->data(), rSize, buffer, buffSize);

	if (dSize != rSize) {
	  printf("error decoding %s : %s \n", filename, ZSTD_getErrorName(dSize));
	  exit(7);
	}

	free(buffer);

	return true;
#else
	return false;
#endif
  }

  const char* get_file_data(size_t* len, const char* filename) {

	const char* ext = strrchr(filename, '.');

	size_t data_len = 0;
	const char* data = nullptr;

	if (strcmp(ext, ".gz") == 0) {
	  // gzipped data.

	  std::vector<char> buf;
	  bool ret = gz_load(&buf, filename);

	  if (ret) {
		char* p = static_cast<char*>(malloc(buf.size() + 1));// @fixme { implement deleter }
		memcpy(p, &buf.at(0), buf.size());
		p[buf.size()] = '\0';
		data = p;
		data_len = buf.size();
	  }

	} else if (strcmp(ext, ".zst") == 0) {
	  printf("zstd\n");
	  // Zstandard data.

	  std::vector<char> buf;
	  bool ret = zstd_load(&buf, filename);

	  if (ret) {
		char* p = static_cast<char*>(malloc(buf.size() + 1));// @fixme { implement deleter }
		memcpy(p, &buf.at(0), buf.size());
		p[buf.size()] = '\0';
		data = p;
		data_len = buf.size();
	  }
	} else {

	  data = mmap_file(&data_len, filename);
	}

	(*len) = data_len;
	return data;
  }

  loadedOBJ LoadObjAndConvert(const char* filename, int num_threads, bool verbose) {
	LOG_S(INFO) << "Loading from " << filename;
	tinyobj_opt::attrib_t attrib;
	std::vector<tinyobj_opt::shape_t> shapes;
	std::vector<tinyobj_opt::material_t> materials;

	auto load_t_begin = std::chrono::high_resolution_clock::now();
	size_t data_len = 0;
	const char* data = get_file_data(&data_len, filename);
	if (data == nullptr) {
	  printf("failed to load file\n");
	  exit(-1);
	}
	LOG_S(INFO) << "File loaded successfully";
	auto load_t_end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> load_ms = load_t_end - load_t_begin;
	if (verbose) {
	  std::cout << "filesize: " << data_len << std::endl;
	  std::cout << "load time: " << load_ms.count() << " [msecs]" << std::endl;
	}
	LOG_S(INFO) << "load time: " << load_ms.count() << " [msecs]";

	tinyobj_opt::LoadOption option;
	option.req_num_threads = num_threads;
	option.verbose = verbose;
	LOG_SCOPE_F(INFO, "Parsing mesh");
	bool ret = parseObj(&attrib, &shapes, &materials, data, data_len, option);

	if (!ret) {
	  std::cerr << "Failed to parse .obj" << std::endl;
	  throw std::runtime_error("Unable to parse .obj");
	}
	LOG_S(INFO) << "vertices: " << attrib.vertices.size();
	LOG_S(INFO) << "indices: " << attrib.indices.size();
	LOG_S(INFO) << "Normals: " << attrib.normals.size();
	LOG_S(INFO) << "TexCoords: " << attrib.texcoords.size();
	LOG_S(INFO) << "MatIds: " << attrib.material_ids.size();
	LOG_S(INFO) << "Materials: " << materials.size();
	LOG_S(INFO) << "Parsed OBJ file successfully";
	return {attrib, shapes, materials};
  }

 public:
  loadedOBJ loadObj(const std::string& filename, u_char threads = -1, bool bVerbose = false) {
	return LoadObjAndConvert(filename.c_str(), threads, bVerbose);
  }
};