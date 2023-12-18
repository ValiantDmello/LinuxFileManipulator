#define _XOPEN_SOURCE 500
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>
#include<ftw.h>

char *src_dir;
char *des_dir;
char *option;
int exts;
char **ext_list;


void mkDir(const char * des){
	char desDir[strlen(des)]; 
	strcpy(desDir, des);
	char *slash = desDir;
	while(slash = strchr(slash + 1, '/')){
		*slash = '\0';
		if(strcmp(desDir, ".") != 0 && strcmp(desDir, "..") != 0){	
			if (mkdir(desDir, 0744) == -1 && errno != EEXIST){
				printf("Error : cant create directory : %s\n", desDir);
				exit(EXIT_FAILURE);
			}	
		}
		*slash = '/';
	}
	if(strcmp(desDir, ".") != 0 && strcmp(desDir, "..") != 0){
		if (mkdir(desDir, 0744) == -1 && errno != EEXIST){
			printf("Error : cant create directory : %s\n", desDir);
			exit(EXIT_FAILURE);
		}
	}
}

void copyFile(const char *source, const char *des){
	int fdSrc, fdDes, bytesRead, bytesWritten, counter = 0; 
	char buffer[1024];
	char *copy = "/copy";
	fdSrc = open(source, O_RDONLY);

	//open source file
	if(fdSrc<0){
		printf("Error : cant open file %s\n", source);
		exit(EXIT_FAILURE);
	}

	//open destination file
	fdDes = open(des, O_CREAT | O_WRONLY | O_TRUNC, 0744);
	if(fdDes<0){
		printf("Error : cant copy file %s\n", des);
		exit(EXIT_FAILURE);
	}
	
	//copy contents from source to destination
	while((bytesRead = read(fdSrc, buffer, 1024))>0){
		bytesWritten = write(fdDes, buffer, bytesRead);	//write to destinantion
		counter++;
		if(bytesWritten < 0){
			printf("Error : cant copy file %s\n", des);
			exit(EXIT_FAILURE);
		}
	}
	
	if(bytesRead < 0){
		printf("Error : cant copy file %s\n", des);
		exit(EXIT_FAILURE);
	}	

	close(fdSrc);
	close(fdDes);
}

int copy(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	if(strcmp(fpath, ".")!=0 && strcmp(fpath, src_dir)!=0){
		char srcFile[strlen(fpath)];
		strcpy(srcFile,fpath);
		char *desFile = srcFile + strlen(src_dir);
		char *des[strlen(desFile) + strlen(des_dir)+1];
		strcpy(des, des_dir);
		// strcat(des, "/");
		strcat(des, desFile);
		//printf("des_dir: %s, desFile: %s, des: %s\n", des_dir, desFile, des);
		if(typeflag == FTW_F){
			if(exts == 0){
				copyFile(fpath, des);
				//printf("copied : %s to %s\n", fpath, des);		
			}else{
				char temp[strlen(desFile)];
				strcpy(temp, desFile);
				char *extension = strrchr(temp, '.');
				//printf("Extension for %s : %s\n", desFile, extension);
				for(int i =0; i<exts; i++){
					if(extension!=NULL){
						if(strcmp(ext_list[i], extension)==0){
						//printf("copying : %s to %s\n", fpath, des);
						copyFile(fpath, des);
					}
					}				
				}
			}
			
		}else if(typeflag == FTW_D){
			mkDir(des);
		}
	}
	return 0;
}

int move(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf){
	if(strcmp(fpath, ".")!=0 && strcmp(fpath, src_dir)!=0){
		char srcFile[strlen(fpath)];
		strcpy(srcFile,fpath);
		char *desFile = srcFile + strlen(src_dir);
		char *des[strlen(desFile) + strlen(des_dir)];
		strcpy(des, des_dir);
		strcat(des, desFile);
		if(typeflag == FTW_F){
			if(exts == 0){
				rename(fpath, des);
				//printf("copied : %s to %s\n", fpath, des);		
			}else{
				char *extension = strrchr(desFile, '.');
				//printf("Extension for %s : %s\n", desFile, extension);
				for(int i =0; i<exts; i++){
					if(extension!=NULL){
						if(strcmp(ext_list[i], extension)==0){
						//printf("copying : %s to %s\n", fpath, des);
						rename(fpath, des);
					}
					}				
				}
			}			
		}else if(typeflag == FTW_D){
			if(exts==0){
				rename(fpath, des);
			}else{
				mkDir(des);
			}
			
		}	
		//printf("moved %d to %d", fpath, desFile);
	}
	return 0;
}

int main(int argc, char *argv[]){
	int ft;

	if(argv[1] == NULL){
		perror("Path to directory doen't exist");
		exit(EXIT_FAILURE);
	}else{
		src_dir = argv[1];
		struct stat sb;
		if(stat(src_dir, &sb)==0 && S_ISDIR(sb.st_mode)){
			//valid directory
		}else{
			perror("Path to directory doen't exist");
			exit(EXIT_FAILURE);
		}
	}

	if(argv[2] == NULL){
		perror("Destination path not specified");
		exit(EXIT_FAILURE);
	}else{
		des_dir = argv[2];
		struct stat sb;
		if(S_ISDIR(sb.st_mode)){
			//If destination folder doesnt exist in hierarchy
			mkDir(des_dir);
		}else{
			perror("Path to directory doen't exist");
			exit(EXIT_FAILURE);
		}
	}

	if(argv[3] == NULL){
		perror("Incorrect option. Expected -cp or -mv");
		exit(EXIT_FAILURE);
	}else{
		option = argv[3];
		//printf("Option: %s\n", option);
	}

	//printf("Des directory: %s\n", des_dir);
	//printf("Source directory: %s\n", src_dir);
	

	if(argc > 4){
		exts = argc - 4;
		ext_list = &argv[4];
		//printf("no of extensions: %d\n", exts);
		//printf("Extension list:  ");
		// for(int k=0; k<exts; k++){
		// 	printf("%s  ", ext_list[k]);
		// }
	}
		
	if(strcmp(option, "-cp") == 0){	
		ft = nftw(src_dir, copy, 100, FTW_PHYS | FTW_MOUNT);
		//printf("copied %s\n", option);
	}else if(strcmp(option, "-mv") == 0){
		ft = nftw(src_dir, move, 100, FTW_PHYS | FTW_MOUNT);
		//printf("moved%s\n", option);
	}else{
		perror("Incorrect option. Expected -cp or -mv");
		exit(EXIT_FAILURE);
	}
}
