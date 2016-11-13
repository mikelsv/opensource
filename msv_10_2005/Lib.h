int ReadDir(MString dir, ISLine &list){

//		char            tmp[MAX_PATH];
        HANDLE          find_handle;
        WIN32_FIND_DATA ff;
		dir;

        find_handle=FindFirstFile(dir,&ff);
        if (find_handle==INVALID_HANDLE_VALUE) return 0;
        while (1)
        {
                if (ff.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
                {
                       // if (*ff.cFileName!='.')
                        {
                                //strcpy(tmp,path);
                                //strcat(tmp,ff.cFileName);
                                //strcat(tmp,"\\");
                           	list.insert(ff.cFileName, 1);
							
							//Dirr(tmp,di,clustersize);
                        }
                }
                else
                {
                        //strcpy(tmp,path);
                        //strcat(tmp,ff.cFileName);
                       // if (strlen(tmp)>75) tmp[75]=0;
//                        printf ("%75c\r%s",' ',tmp); // \r
//						if (ff.cFileName!="thumbs.db")
						list.insert(ff.cFileName, 0);
//
//                        di->filecount++;
//                        di->filesize+=(((_int64)ff.nFileSizeHigh)<<32)+ff.nFileSizeLow;
//                       if (ff.dwFileAttributes&FILE_ATTRIBUTE_COMPRESSED)
//                        {
//                                strcpy(tmp,path);
//                                strcat(tmp,ff.cFileName);
//                                s=GetCompressedFileSize(tmp,NULL);
//                        }
//                        else s=(((_int64)ff.nFileSizeHigh)<<32)+ff.nFileSizeLow;
//                        di->filerealsize+=s;
//                        s1=s/clustersize;
//                        if (s%clustersize) s1++;
//                        di->filedisksize+=s1*clustersize;
//                        di->usedclusters+=s1;
                }
                if (!FindNextFile(find_handle,&ff)) break;
        }
        FindClose(find_handle);




return 1;
}