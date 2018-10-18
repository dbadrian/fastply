#pragma once
#include <string>
#include <climits>
#include <cstdint>
#include <cstddef>
#include <map>
#include <fstream>
#include <sstream>

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>


namespace fastply {

static_assert(CHAR_BIT == 8);
static_assert(CHAR_BIT * sizeof (float) == 32);

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

template<typename T>
struct PlyElement {
    constexpr static std::size_t sizeT = {sizeof(T)};
    int size = 0;
    T* start = nullptr;
    T* end = nullptr;
};

template<typename T>
constexpr std::size_t PlyElement<T>::sizeT;

template<typename VertexLayout, typename FaceLayout>
class FastPly
{
public:
    FastPly() = default;
    ~FastPly() {
        if(fd_ != -1) {
            close(fd_);
        }
    };

    FastPly(const FastPly&) = delete;
    FastPly& operator=(const FastPly&) = delete;

    bool init(std::string path)
    {
        path_ = path;
        {
            std::ifstream ss(path_, std::ios::binary);
            if (ss.fail()) throw std::runtime_error("Failed to open " + path_);

            std::string line;
            while (std::getline(ss, line))
            {
                std::istringstream ls(line);
                std::string token;
                ls >> token;
                if (token == "ply" || token == "PLY" || token == "") continue;
                else if (token == "comment")    continue;
                else if (token == "format")     {if(!read_header_format(ls)) return false;}
                else if (token == "element")    {if(!read_header_element(ls)) return false;}
                else if (token == "property")   {if(!read_header_property(ls)) return false;}
                else if (token == "obj_info")   continue; //if(!read_header_text(line, ls, objInfo, 9)) return false;
                else if (token == "end_header") break;
                else return false; // unexpected header field
            }
            header_offset_ = ss.tellg();
        }

        // memory map open the file
        if ((fd_ = open(path_.c_str(), O_RDONLY, 0)) == -1)
            throw std::runtime_error("Failed to open " + path_);

        // get filesize
        auto file_size = getFilesize(path_.c_str());

        mapped_data = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd_, 0);
        if (mapped_data == MAP_FAILED) {
            close(fd_);
            perror("Error mmapping the file");
            exit(EXIT_FAILURE);
        }

        vertex_header_.start = reinterpret_cast<VertexLayout*>(static_cast<char*>(mapped_data) + header_offset_);
        vertex_header_.end = vertex_header_.start + vertex_header_.size;
        // face_header_.offset = vertex_header_.offset + vertex_header_.size * vertex_header_.sizeT;
        header_parsed_ = true;
        return true;
    }

    void shutdown() {
        if(fd_ != -1) {
            close(fd_);
            fd_ = -1;
            mapped_data = nullptr;
        }
    }

    std::string getInputPath() const {
        return path_;
    }

    int getHeaderOffset() const {
        return header_offset_;
    }

    bool isBigEndian() const {
        return is_big_endian_;
    }

    bool isHeaderParsed() const {
        return header_parsed_;
    }

    PlyElement<VertexLayout> getVertexHeader() {
        return vertex_header_;
    }

    PlyElement<FaceLayout> getFaceHeader() {
        return face_header_;
    }

private:

    bool read_header_format(std::istream& is)
    {
        std::string s;
        (is >> s);
        if (s == "binary_little_endian") is_big_endian_ = false;
        else if (s == "binary_big_endian") is_big_endian_ = true;
        else return false;
        return true;
    }

    bool read_header_element(std::istream& is) {
        std::string s;
        (is >> s);
        if (s == "vertex") {
            (is >> s);
            vertex_header_.size = std::stoi(s);
        } else if (s == "face") {
            (is >> s);
            face_header_.size = std::stoi(s);
        } else {
            throw std::runtime_error("Unsupported Element type " + s);
        }
        return true;
    }
    bool read_header_property(std::istream& is) {return true;}
    bool read_header_text(std::istream& is) {return true;}

    std::string path_ = "";
    bool header_parsed_ = false;
    int header_offset_ = -1;
    bool is_big_endian_ = false;

    PlyElement<VertexLayout> vertex_header_;
    PlyElement<FaceLayout> face_header_;

    int fd_ = -1;
    void* mapped_data = nullptr;
};
} // end of fastply namespace
