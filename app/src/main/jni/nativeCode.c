#include <stdio.h>
#include <jni.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <netdb.h>

/**
* Change current working directory
*/
int Java_com_illinois_cs241_simplesh_NativeClass_chdir(JNIEnv* env, jobject this, jstring input)
{
    char * path = (*env)->GetStringUTFChars(env, input, 0);
    int retval = chdir(path);
    (*env)->ReleaseStringUTFChars(env, input, path);
    return retval;
}

/**
* Get current working directory
*/
jbyteArray Java_com_illinois_cs241_simplesh_NativeClass_getcwd(JNIEnv* env, jobject this)
{
    char dir[1024];
    getcwd(dir, 1024);

    jbyteArray ret =(*env)->NewByteArray(env, strlen(dir)+1);
    (*env)->SetByteArrayRegion(env, ret, 0, strlen(dir)+1, dir);
    return ret;
}

/**
* Get machine's hostname
*/
jbyteArray Java_com_illinois_cs241_simplesh_NativeClass_getHostname(JNIEnv* env, jobject this)
{

    char name[1024];
    gethostname(name, 1024);
    jbyteArray ret =(*env)->NewByteArray(env, strlen(name)+1);
    (*env)->SetByteArrayRegion(env, ret, 0, strlen(name)+1, name);
    return ret;

    //@todo: Code to get fully qualified hostname
   /* struct addrinfo hints={0};
    hints.ai_family=AF_UNSPEC;
    hints.ai_flags=AI_CANONNAME;

    struct addrinfo* res=0;
    if (getaddrinfo(name,0,&hints,&res)==0) {
        // The hostname was successfully resolved.
        printf("%s\n",res->ai_canonname);
        freeaddrinfo(res);

        jbyteArray ret =(*env)->NewByteArray(env, strlen(res->ai_canonname)+1);
              //(*env)->SetByteArrayRegion (ret, 0, 1024, buf);
              (*env)->SetByteArrayRegion(env, ret, 0, strlen(res->ai_canonname)+1, res->ai_canonname);
              return ret;
    } else {
        // Not resolved, so fall back to hostname returned by OS.
        printf("%s\n",name);

      jbyteArray ret =(*env)->NewByteArray(env, strlen(name)+1);
      //(*env)->SetByteArrayRegion (ret, 0, 1024, buf);
      (*env)->SetByteArrayRegion(env, ret, 0, strlen(name)+1, name);
      return ret;
    }*/

}

/**
* Get current user's name
*/
jbyteArray Java_com_illinois_cs241_simplesh_NativeClass_getUserName(JNIEnv* env, jobject this){

  register struct passwd *pw;
  register uid_t uid;
  int c;

  uid = geteuid ();
  pw = getpwuid (uid);
  if(pw){

      jbyteArray ret =(*env)->NewByteArray(env, strlen(pw->pw_name)+1);
      //(*env)->SetByteArrayRegion (ret, 0, 1024, buf);
      (*env)->SetByteArrayRegion(env, ret, 0, strlen(pw->pw_name)+1, pw->pw_name);
      return ret;
  } else {
      jbyteArray ret =(*env)->NewByteArray(env, strlen("user")+1);
      (*env)->SetByteArrayRegion(env, ret, 0, strlen("user")+1, "user");
      return ret;
  }
}

/**
* Execute a program
*/
jbyteArray Java_com_illinois_cs241_simplesh_NativeClass_runCommand(JNIEnv* env, jobject this, jstring input)
{
    char * program = (*env)->GetStringUTFChars(env, input, 0);//Convert Java string to c-string
    char ** argv = (char**)realloc(argv, sizeof(char*));//Allocate space for arguments

    //Split input string into argument list
    int numtokens = 1;
    char *token = strtok(program, " ");
    argv[0] = token;

    /* walk through other tokens */
    while( token != NULL )
    {
        token = strtok(NULL, " ");
        numtokens++;
        argv = (char**)realloc(argv, sizeof(char*)*numtokens);
        argv[numtokens-1] = token;

    }

    //Open a pipe to read the executed program's standard output
    int p[2];
    pipe(p);
    int status;

    int buffer_size = 10240;//Large buffer for stdout, @todo: should be malloc'd.
    ssize_t bytes_read;

    //Fork and exec
    char buf[buffer_size];
    pid_t pid = fork();
    if(pid == 0){//in child
        dup2(p[1], 1); // redirect the output (STDOUT to the pipe)
        close(p[0]);

        //Debug prints
        printf("Executing %s! With arguments:\n", program);
        char ** temp = argv;
        while(*temp != NULL){
            puts(*temp);
            temp++;
        }
        execvp(program, argv);
        printf("Exec failed!\n");
        exit(1);

    } else {
        close(p[1]);
        waitpid(pid, &status, WUNTRACED | WCONTINUED);//Wait for child to finish
        bytes_read = read(p[0], buf, buffer_size);//Read from child's stdout
    }

    (*env)->ReleaseStringUTFChars(env, input, program); // Release java string, not entirely sure what this does.

    //Return program's output to Java
    jbyteArray ret =(*env)->NewByteArray(env, bytes_read);
    (*env)->SetByteArrayRegion(env, ret, 0, bytes_read, buf);
    return ret;
}