
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
    fprintf(stderr, "usage: falloc [-e] filesize filename\n"
        "where  -e  extend the file from the current end.\n"
        "           Without -e, the system call makes sure the entire file is fleshed\n"
        "           out, so it would take longer when used on an existing file.\n"
        "           Always use -e if you are confident the file is not sparse.\n"
        "\n"
        "filesize can have multiplier suffixes inspired from dd:\n"
        "\n"
        "    c=1, w=2, b=512, K=1024, M=1024*1024, G=1024*1024*1024,\n"
        "    kB=1000, MB=1000*1000, GB=1000*1000*1000\n"
        "    p=IDS page (2K), p##=page of size ##K eg p=4 => 4K\n"
        "\n"
);

    exit(1);
}


void fatal(char *str) {
    perror(str);
    exit(1);
}


int main(int argc, char *argv[]) {
    int fd, i, opt;
    off_t ofs = 0, size = 0;
    int end_flag = 0;

    while((opt = getopt(argc, argv, "e")) != -1) {
        switch(opt) {
          case 'e':
            end_flag = 1;
            break;

          default:
            usage();
            exit(1);
        }
    }

    if(argc - optind != 2)
        usage();

    /* first real arg has to be the length, which can take */
    /* a few multiplicative suffixes borrowed from dd      */
    size = lengthSpec(argv[optind]);
    assert(size > 0);

    /* create the file, if necessary with Informixly useful mode */
    /* TODO: probably should add an option to control this       */
    fd = open(argv[optind + 1], O_CREAT | O_RDWR, 0660);
    if(fd < 0)
        fatal("open");

    if(end_flag) {
        /* calculate offset and adjust size to just grow the file from the current end */
        ofs = lseek(fd, (off_t) 0, SEEK_END);
        if(ofs == (off_t) -1)
            fatal("lseek");
        assert(ofs >= 0);
        size -= ofs;
    }

    /* if there's no growth to be made, drop out */
    /* otherwise it will look like an error */
    if(size <= 0)
        exit(0);

    /* this thing returns it's error code instead of setting errno */
    /* but the codes are legitimate for errno. Strange but true    */
    if((i = posix_fallocate(fd, ofs, size))) {
        /* put the error code where it belongs! */
        errno = i;
        fatal("posix_fallocate");
    }

    close(fd);
    exit(i != 0);
}   /* main */

