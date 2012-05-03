
# define        _XOPEN_SOURCE       600

# include       <assert.h>
# include       <fcntl.h>
# include       <stdlib.h>
# include       <stdio.h>
# include       <unistd.h>
# include       <string.h>
# include       <sys/types.h>
# include       <sys/stat.h>
# include       <errno.h>
# include       "lspec.h"


void usage() {
    fprintf(stderr,
        "usage: truncate filesize filename\n"

        "filesize can have multiplier suffixes inspired from dd:\n"
        "\n"
        "    c=1, w=2, b=512, K=1024, M=1024*1024, G=1024*1024*1024,\n"
        "    kB=1000, MB=1000*1000, GB=1000*1000*1000\n"
        "    p=IDS page (2K), p##=page of size ##K eg p=4 => 4K\n"
        "\n"
        "WARNING: THIS IS A VERY DANGEROUS PROGRAM!! GET IT RIGHT!!!\n"
        "WARNING: THIS IS A VERY DANGEROUS PROGRAM!! GET IT RIGHT!!!\n"
        "WARNING: THIS IS A VERY DANGEROUS PROGRAM!! GET IT RIGHT!!!\n"
        "WARNING: THIS IS A VERY DANGEROUS PROGRAM!! GET IT RIGHT!!!\n"
        "\n"
);
    exit(1);
}


void fatal(char *str) {
    perror(str);
    exit(1);
}


int main(int argc, char *argv[]) {
    int fd, i;
    off_t ofs = 0, size = 0;

    if(argc - optind != 2)
        usage();

    size = lengthSpec(argv[optind]);
    assert(size >= 0);

    fd = open(argv[optind + 1], O_RDWR);
    if(fd < 0)
        fatal("open");

    ofs = lseek(fd, (off_t) 0, SEEK_END);
    if(ofs == (off_t) -1)
        fatal("lseek");
    assert(ofs >= 0);

    if(size > ofs) {
        fprintf(stderr, "requested size would extend the file\n");
        exit(1);
    }

    i = ftruncate(fd, size);
    if(i)
        fatal("truncate");

    close(fd);
    exit(i != 0);
}

