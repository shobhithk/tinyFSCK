#pragma once

#include <string>
#include <sys/types.h>
#include <fcntl.h>

class Filesystem {
public:
    explicit Filesystem(std::string& path, unsigned int flags = O_RDWR, mode_t mode = 0);
    
    // Make Filesystem object move only. RAII memes :^)
    Filesystem(Filesystem&) = delete;
    Filesystem(Filesystem&& fs) {
        operator=(std::move(fs));
    }
    Filesystem& operator=(Filesystem&) = delete;
    Filesystem& operator=(Filesystem&& fs) {
        fs_disc = fs.fs_disc;
        fs_size = fs.fs_size;
        fs.fs_disc = fs.fs_size = 0; // Invalidate object from which we moved.
        return *this;
    }

    ~Filesystem();

    inline off64_t size() { return fs_size; }

    void read_at(off64_t offset, char* buffer, off64_t size);
    void write_at(off64_t offset, char* buffer, off64_t size);
    
private:
    int fs_disc;
    off64_t fs_size;
    struct Superblock {
        off64_t blocks_count;
        off64_t block_size;
        off64_t blocks_per_group;
        off64_t inode_size;
        off64_t inodes_per_group;
        off64_t reserved_group_desciption_block;
    } fs_superblock;

    enum seek_origin {
        CUR = SEEK_CUR,
        SET = SEEK_SET,
        END = SEEK_END
    };

    void raw_read(char *buffer, off64_t size);
    void raw_seek(off64_t offset, enum seek_origin origin);
    void raw_write(char *buffer, off64_t size);


    void read_superblock();
};