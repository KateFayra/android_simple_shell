package com.illinois.cs241.simplesh;

public class NativeClass {
    static {
        System.loadLibrary("ndkModule");
    }

    public native byte[] runCommand(String program);
    public native byte[] getUserName();
    public native byte[] getcwd();
    public native byte[] getHostname();
    public native int chdir(String path);
}
