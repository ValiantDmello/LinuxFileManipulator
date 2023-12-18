#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <ftw.h>

#define MAX_FTW_DEPTH 16
#define MAX_EXTENSIONS 6

const char *source;
const char *dest;
const char *allowed_extensions[MAX_EXTENSIONS];

int copy_or_move(const char *src, const char *dst, int move, const char *allowed_extensions[MAX_EXTENSIONS]);

int copy_file(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        // printf("Extension : %s", ext);
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        else
        {
            char new_path[1024];
            sprintf(new_path, "%s/%s", dest, fpath);
            printf("%s - fpath\n", fpath);
            FILE *src, *dst;
            src = fopen(fpath, "r");
            dst = fopen(new_path, "w");

            if (!src || !dst)
            {
                perror("fopen src");
                printf("Error: Could not open files.\n");
                exit(EXIT_FAILURE);
            }

            char buffer[4096];
            size_t n_read;
            while ((n_read = fread(buffer, 1, sizeof buffer, src)) > 0)
                fwrite(buffer, 1, n_read, dst);

            fclose(src);
            fclose(dst);

            return 0;
        }
    }
    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

int copy_directory(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        else
        {
            char new_path[1024];
            sprintf(new_path, "%s/%s", dest, fpath);
            printf("");
            switch (typeflag)
            {
            case FTW_D:
                mkdir(new_path, 0700);
                break;
            case FTW_F:
            {
                FILE *src, *dst;
                src = fopen(fpath, "r");
                dst = fopen(new_path, "w");

                if (!src || !dst)
                {
                    perror("fopen src");
                    printf("Error: Could not open files.\n");
                    exit(EXIT_FAILURE);
                }

                char buffer[4096];
                size_t n_read;
                while ((n_read = fread(buffer, 1, sizeof buffer, src)) > 0)
                    fwrite(buffer, 1, n_read, dst);

                fclose(src);
                fclose(dst);

                break;
            }
            default:
                printf("Error: Unexpected typeflag.\n");
                exit(EXIT_FAILURE);
            }

            return 0;
        }
    }
    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

int move_file(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                // printf("Break");
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                printf("%s - ext, %s - allowed_extensions ,%d - strcmp", ext, allowed_extensions[i], strcmp(ext, allowed_extensions[i]));
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        else
        {
            char new_path[1024];
            // strncpy(new_path, "./dst/", sizeof dest);
            // strncat(new_path, fpath + 5, sizeof dest - strlen(dest) - 1);
            // sprintf(new_path, "%s/%s", dest, fpath);
            // printf("%s - New Path\n", new_path);
            int result = rename(fpath, dest);
            printf("%d - result", result);
            // printf("%d - File moved successfully");
            if (result != 0)
            {
                printf("Error: Could not move file.\n");
                perror("rename");
                exit(EXIT_FAILURE);
            }

            return 0;
        }
    }
    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

int move_directory(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                // printf("%s - ext, %s - allowed_extensions ,%d - strcmp", ext, allowed_extensions[i], strcmp(ext, allowed_extensions[i]));
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        else
        {
            // Move the file if its extension is in the list of allowed extensions
            char new_path[1024];
            // strncpy(new_path, dest, sizeof dest);
            // strncat(new_path, fpath + 5, sizeof dest - strlen(dest) - 1);
            sprintf(new_path, "%s", dest);
            printf("%s - New Path\n", new_path);
            int result = rename(fpath, new_path);
            printf("%d - result\n", result);
            if (result != 0)
            {
                printf("Error: Could not move file.\n");
                perror("rename");
                exit(EXIT_FAILURE);
            }

            return 0;
        }
    }
    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

/////////////////////////////////////////////////

int copy_file_or_directory(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        // else
        // {
        char file_name[4096];
        char newpath[4096];
        char *last_slash = strrchr(fpath, '/');
        if (last_slash == NULL)
        {
            strcpy(file_name, fpath);
        }
        else
        {
            strcpy(file_name, last_slash + 1);
        }
        printf("%s\n", file_name);
        sprintf(newpath, "%s/%s", dest, file_name);
        printf("%s\n", newpath);
        char new_path[1024];
        sprintf(new_path, "%s/%s", dest, fpath);
        switch (typeflag)
        {
        case FTW_D:
            mkdir(new_path, 0700);
            break;
        case FTW_F:
        {
            FILE *src, *dst;
            src = fopen(fpath, "r");
            dst = fopen(newpath, "w");
            if (!src || !dst)
            {
                printf("Error: Could not open files.\n");
                exit(EXIT_FAILURE);
            }

            char buffer[4096];
            size_t n_read;
            while ((n_read = fread(buffer, 1, sizeof buffer, src)) > 0)
                fwrite(buffer, 1, n_read, dst);

            fclose(src);
            fclose(dst);

            break;
        }
        default:
            printf("Error: Unexpected typeflag.\n");
            exit(EXIT_FAILURE);
        }

        return 0;
    }

    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

int move_file_or_directory(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    if (typeflag == FTW_F)
    { // Check if this is a regular file
        // Get the file extension
        const char *ext = strrchr(fpath, '.');
        ext++;
        if (ext == NULL)
        {
            // The file does not have an extension, so skip it
            return 0;
        }

        // Check if the file extension is in the list of allowed extensions
        int allowed = 0;
        for (int i = 0; i < MAX_EXTENSIONS; i++)
        {
            if (allowed_extensions[i] == NULL)
            {
                break;
            }
            if (strcmp(ext, allowed_extensions[i]) == 0)
            {
                // printf("%s - ext, %s - allowed_extensions ,%d - strcmp", ext, allowed_extensions[i], strcmp(ext, allowed_extensions[i]));
                allowed = 1;
                break;
            }
        }
        if (!allowed)
        {
            // The file extension is not in the list of allowed extensions, so skip it
            printf("%s is not an allowed extension", ext);
            return 0;
        }
        else
        {
            char file_name[4096];
            char newpath[4096];
            char *last_slash = strrchr(fpath, '/');
            if (last_slash == NULL)
            {
                strcpy(file_name, fpath);
            }
            else
            {
                strcpy(file_name, last_slash + 1);
            }
            printf("%s\n", file_name);
            sprintf(newpath, "%s/%s", dest, file_name);
            struct stat st = {0};
            if (stat(dest, &st) == -1)
            {
                if (mkdir(newpath, 0700) == -1)
                {
                    perror("mkdir");
                    return 1;
                }
            }
            // printf("%s\n", newpath);
            char new_path[1024];
            sprintf(new_path, "%s/%s", dest, newpath);
            // Move the file if its extension is in the list of allowed extensions
            // char new_path[1024];
            // strncpy(new_path, dest, sizeof dest);
            // strncat(new_path, fpath + 5, sizeof dest - strlen(dest) - 1);
            // sprintf(new_path, "%s", dest);
            // printf("%s - New Path\n", new_path);
            int result = rename(fpath, new_path);
            // printf("%d - result\n", result);
            if (result != 0)
            {
                printf("Error: Could not move file.\n");
                perror("rename");
                exit(EXIT_FAILURE);
            }

            return 0;
        }
    }
    else
    {
        // This is not a regular file, so skip it
        return 0;
    }
}

////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s <source directory> <destination directory> <cp/mv> <file extensions>\n", argv[0]);
        return 1;
    }

    source = argv[1];
    dest = argv[2];
    int move = (strcmp(argv[3], "mv") == 0);
    int extension_count = argc - 4;
    if (extension_count <= MAX_EXTENSIONS)
    {
        for (int i = 0; i < extension_count; i++)
        {
            allowed_extensions[i] = argv[i + 4];
            // printf("%d) %s\n",i+1,allowed_extensions[i]);
        }
    }
    else
    {
        printf("Maximum %d extensions are allowed", MAX_EXTENSIONS);
        return 1;
    }

    return copy_or_move(source, dest, move, allowed_extensions);
}

int copy_or_move(const char *src, const char *dst, int move, const char *allowed_extensions[MAX_EXTENSIONS])
{
    struct stat src_stat;
    if (stat(src, &src_stat) == -1)
    {
        printf("Error: Cannot access source directory %s\n", src);
        return 1;
    }

    if (move)
    {
        nftw(src, move_file_or_directory, MAX_FTW_DEPTH, FTW_NS);
        return 0;
        // if (S_ISREG(src_stat.st_mode))
        // {
        //     nftw(src, move_file, MAX_FTW_DEPTH, FTW_NS);
        //     return 0;
        //     // return copy_or_move_file(src, dst, move);
        // }
        // else if (S_ISDIR(src_stat.st_mode))
        // {
        //     nftw(src, move_directory, MAX_FTW_DEPTH, FTW_NS);
        //     return 0;
        //     // return copy_or_move_directory(src, dst, move);
        // }
        // else
        // {
        //     printf("Error: %s is not a file or directory\n", src);
        //     return 1;
        // }
    }
    else
    {
        nftw(src, copy_file_or_directory, MAX_FTW_DEPTH, FTW_NS);
        return 0;
        // if (S_ISREG(src_stat.st_mode))
        // {
        //     nftw(src, copy_file, MAX_FTW_DEPTH, FTW_NS);
        //     return 0;
        //     // return copy_or_move_file(src, dst, move);
        // }
        // else if (S_ISDIR(src_stat.st_mode))
        // {
        //     nftw(src, copy_directory, MAX_FTW_DEPTH, FTW_NS);
        //     return 0;
        //     // return copy_or_move_directory(src, dst, move);
        // }
        // else
        // {
        //     printf("Error: %s is not a file or directory\n", src);
        //     return 1;
        // }
    }
}
