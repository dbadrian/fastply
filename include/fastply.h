#ifndef FASTPLY_H
#define FASTPLY_H

#include <algorithm>
#include <string>
#include <cstdint>
#include <cstddef>
#include <tuple>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <system_error>
#include <memory>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace fastply {

size_t get_filesize(const std::string& filename) {
    struct ::stat st;
    stat(filename.c_str(), &st);
    return st.st_size;
}

template<typename T>
struct PlyElement {

    T const & operator [](std::size_t i) const { return start[i]; }

    T const & at(std::size_t  i) const {
        if(static_cast<unsigned int>(i) < size){
            return start[i];
        }
        else {
            throw std::out_of_range("accessed position is out of range");
        }
    }

    constexpr static std::size_t sizeT = {sizeof(T)};
    std::size_t size = 0;
    T const * start = nullptr;
    T const * end = nullptr;
};

template<typename T>
constexpr std::size_t PlyElement<T>::sizeT;

template<typename... Args>
class FastPly
{
public:
    FastPly() = default;
    ~FastPly() {
        close();
    };

    FastPly(const FastPly&) = delete;
    FastPly& operator=(const FastPly&) = delete;

    bool init(std::string path)
    {
        if(!n_element_types_) return false;
        path_ = path;
        // Parse Header: This will only query the basic information
        // such as little/big endian encoding, how many elements etc.
        if(!parseHeader()) return false;

        // memory map open the file
        fd_ = ::open(path_.c_str(), O_RDONLY, 0);
        if (fd_ == -1) throw std::system_error(EFAULT, std::generic_category());

        // get filesize
        auto file_size = get_filesize(path_.c_str());

        mapped_data = mmap(0, file_size, PROT_READ, MAP_PRIVATE, fd_, 0);
        if (mapped_data == MAP_FAILED) {
            ::close(fd_);
            throw std::runtime_error("Failed to memory map " + path_);
        }

        // Build the PlyElements
        if constexpr (sizeof...(Args) > 0) {
            setupElements<Args...>();
        }

        return true;
    }


    void close() {
        if(fd_ != -1) {
            ::close(fd_);
            fd_ = -1;
            mapped_data = nullptr;
        }
    }

    std::string getInputPath() const {
        return path_;
    }

    int getHeaderOffset() const {
        return header_length_;
    }

    bool isBigEndian() const {
        return is_big_endian_;
    }

    bool isHeaderParsed() const {
        return header_parsed_;
    }

    std::size_t numberElements() const {
        return n_element_types_;
    }

    const std::tuple<Args...>& getElements() const {
        return elements_;
    }

    template<typename T>
    const auto get() const {
        return std::get<T>(elements_);
    }

private:

    bool parseHeader() {
        std::ifstream ss(path_, std::ios::binary);
        if (ss.fail()) throw std::system_error(EFAULT, std::generic_category());

        std::string line;
        while (std::getline(ss, line))
        {
            std::istringstream ls(line);
            std::string keyword;
            ls >> keyword;
            // This transformation to all lower chars only handles ASCII.
            // Ply files should only be ASCII (+binary), so it should be alright.
            // Note: This allows slightly non-standard formats to be successfully parsed.
            // aka PlY plY cOmMent, will be fine.
            std::transform(keyword.begin(), keyword.end(), keyword.begin(), ::tolower);
            if (keyword == "ply" || keyword == "") continue;
            else if (keyword == "comment")    continue;
            else if (keyword == "format")     { if(!readEncoding(ls)) return false; }
            else if (keyword == "element")    { if(!this->getElementCount(ls)) return false; }
            else if (keyword == "property")   continue;
            else if (keyword == "obj_info")   continue;
            else if (keyword == "end_header") break;
            else {
                throw std::runtime_error("Unknown keyword '" + keyword + "' found");
                return false; // exception or bool unexpected header field
            }
        }
        header_length_ = ss.tellg();
        header_parsed_ = true;
        return true;
    }

    bool readEncoding(std::istream& is)
    {
        std::string s;
        (is >> s);
        if (s == "binary_little_endian") is_big_endian_ = false;
        else if (s == "binary_big_endian") is_big_endian_ = true;
        else return false; // No support for ascii or typos ;).
        return true;
    }

    bool getElementCount(std::istream& is) {
        // If we read more element definition keywords than defined
        if(element_definition_count >= n_element_types_) {
            throw std::runtime_error("Definition of PLY file does not match the loaded file. Too many element types found.");
        }

        std::string s;
        (is >> s); // s contains element name
        // element_name_[element_definition_count] = s;

        // Insert information about element count into the element header
        (is >> s); // s contains element count as string
        element_count_[element_definition_count] = std::stoi(s); //convert to number

        ++element_definition_count; // update the count of read elements
        return true;
    }

    template<typename T, typename... Ts>
    void setupElements() {
        // Index of current element to setup
        constexpr int idx = sizeof...(Args) - sizeof...(Ts) - 1;

        // Get element and fill in the size
        auto& el = std::get<idx>(elements_);
        el.size = element_count_[idx];

        unsigned char const * start;
        if constexpr (idx == 0) {
            start = static_cast<unsigned char const *>(mapped_data) + header_length_;
        } else {
            start = reinterpret_cast<unsigned char const *>(std::get<(idx-1)>(elements_).end);
        }

        el.start = reinterpret_cast<decltype(el.start)>(start);
        el.end = el.start+el.size;

        if constexpr (sizeof...(Ts) > 0) {
            setupElements<Ts...>();
        }
    }

    std::string path_ = ""; //<!-- Path to input pl
    bool is_big_endian_ = false; //<!-- Encoding of ply fily
    unsigned int element_definition_count = 0; //<!-- Elements seen in Ply header
    int header_length_ = -1; //<!-- Length of header
    bool header_parsed_ = false; //<!-- Whether the header was successfully parsed

    std::tuple<Args...> elements_; //<!-- Element layouts in order as given as template params
    const unsigned int n_element_types_ = sizeof...(Args); //<!-- Number of template params (known at compile time)
    unsigned int element_count_[std::tuple_size<std::tuple<Args...>>::value] = {}; //<!-- How many entries per element exist
    // std::string element_name_[std::tuple_size<std::tuple<Args...>>::value] = {""}; //<!-- Name of seen elements. Not used

    int fd_ = -1; //<!-- File descriptor pointing to the ply file
    void* mapped_data = nullptr; //<!-- Ptr to the memory-mapped start location of the ply file
};


} // End of fastply namespace

#endif /* FASTPLY_H */