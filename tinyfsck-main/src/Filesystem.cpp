#include "Filesystem.h"

#include <stdexcept>

#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>

#include <ext2fs/ext2fs.h>

Filesystem::Filesystem(std::string& path, unsigned int flags, mode_t mode) {
    flags |= O_LARGEFILE;
    fs_disc = open(
        path.c_str(),
        flags,
        mode
    );

    struct stat64 fs_stat {};
    fstat64(fs_disc, &fs_stat);
    if (fs_stat.st_mode & S_IFBLK) {
        ioctl(fs_disc, BLKGETSIZE64, &fs_size);
    } else {
        fs_size = fs_stat.st_size;
    }

    read_superblock();
}

Filesystem::~Filesystem() {
    close(fs_disc);
}

void Filesystem::raw_read(char *buffer, off64_t size) {
    errno = 0;
    if (read(fs_disc, buffer, size) < 0) {
        if (errno != EINTR)
            throw std::runtime_error("Failed to read from filesystem");
        perror("read");
        errno = 0;
    }
}

void Filesystem::raw_seek(off64_t offset, Filesystem::seek_origin origin) {
    errno = 0;
    if (lseek64(fs_disc, offset, origin) < 0) {
        if (errno != EINTR)
            throw std::runtime_error("Failed to seek the filesystem");
        perror("lseek64");
        errno = 0;
    }
}

void Filesystem::raw_write(char *buffer, off64_t size) {
    errno = 0;
    if (write(fs_disc, buffer, size) < 0) {
        if (errno != EINTR)
            throw std::runtime_error("Failed to write to the filesystem");
        perror("write");
        errno = 0;
    }
}

void Filesystem::read_at(off64_t offset, char* buffer, off64_t size) {
    raw_seek(offset, Filesystem::seek_origin::SET);
    raw_read(buffer, size);
}

void Filesystem::write_at(off64_t offset, char* buffer, off64_t size) {
    raw_seek(offset, Filesystem::seek_origin::SET);
    raw_write(buffer, size);
}

void Filesystem::read_superblock() {
    if (SUPERBLOCK_OFFSET + SUPERBLOCK_SIZE > fs_size)
        throw std::runtime_error("Filesystem too small to contain a ext2 superblock");
    ext2_super_block sb;
    read_at(SUPERBLOCK_OFFSET, (char*) &sb, sizeof(sb));
    
    if (sb.s_magic != EXT2_SUPER_MAGIC)
        throw std::runtime_error("Filesystem does not contain ext2 magic number. "
                                 "It is probably not an ext2 filesystem.");
    
    fs_superblock.blocks_count = sb.s_blocks_count;
    fs_superblock.block_size = EXT2_BLOCK_SIZE(&sb);
    fs_superblock.blocks_per_group = sb.s_blocks_per_group;
    fs_superblock.inode_size = sb.s_inode_size;
    fs_superblock.inodes_per_group = sb.s_inodes_per_group;
    fs_superblock.reserved_group_desciption_block = sb.s_reserved_gdt_blocks;
}