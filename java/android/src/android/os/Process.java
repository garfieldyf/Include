package android.os;

public class Process {
    public static final int SYSTEM_UID = 1000;
    public static final int PHONE_UID = 1001;
    public static final int FIRST_APPLICATION_UID = 10000;
    public static final int LAST_APPLICATION_UID = 19999;
    public static final int THREAD_PRIORITY_DEFAULT = 0;
    public static final int THREAD_PRIORITY_LOWEST = 19;
    public static final int THREAD_PRIORITY_BACKGROUND = 10;
    public static final int THREAD_PRIORITY_FOREGROUND = -2;
    public static final int THREAD_PRIORITY_DISPLAY = -4;
    public static final int THREAD_PRIORITY_URGENT_DISPLAY = -8;
    public static final int THREAD_PRIORITY_AUDIO = -16;
    public static final int THREAD_PRIORITY_URGENT_AUDIO = -19;
    public static final int THREAD_PRIORITY_MORE_FAVORABLE = -1;
    public static final int THREAD_PRIORITY_LESS_FAVORABLE = +1;
    public static final int SIGNAL_QUIT = 3;
    public static final int SIGNAL_KILL = 9;
    public static final int SIGNAL_USR1 = 10;

    public static final native long getElapsedCpuTime();

    public static final int myPid() {
        throw new RuntimeException("Stub!");
    }

    public static final int myPpid() {
        throw new RuntimeException("Stub!");
    }

    public static final int myTid() {
        throw new RuntimeException("Stub!");
    }

    public static final int myUid() {
        throw new RuntimeException("Stub!");
    }

    public static final UserHandle myUserHandle() {
        throw new RuntimeException("Stub!");
    }

    public static final native int getUidForName(String name);

    public static final native int getGidForName(String name);

    public static final int getUidForPid(int pid) {
        throw new RuntimeException("Stub!");
    }

    public static final int getParentPid(int pid) {
        throw new RuntimeException("Stub!");
    }

    public static final int getThreadGroupLeader(int tid) {
        throw new RuntimeException("Stub!");
    }

    public static final native void setThreadPriority(int tid, int priority) throws IllegalArgumentException, SecurityException;

    public static final native void setThreadGroup(int tid, int group) throws IllegalArgumentException, SecurityException;

    public static final native void setProcessGroup(int pid, int group) throws IllegalArgumentException, SecurityException;

    public static final native int getProcessGroup(int pid) throws IllegalArgumentException, SecurityException;

    public static final native void setThreadPriority(int priority) throws IllegalArgumentException, SecurityException;

    public static final native int getThreadPriority(int tid) throws IllegalArgumentException;

    public static final native void setThreadScheduler(int tid, int policy, int priority) throws IllegalArgumentException;

    @Deprecated
    public static final boolean supportsProcesses() {
        throw new RuntimeException("Stub!");
    }

    public static final void killProcess(int pid) {
        throw new RuntimeException("Stub!");
    }

    public static final native int setUid(int uid);

    public static final native int setGid(int uid);

    public static final native void sendSignal(int pid, int signal);

    public static final void killProcessQuiet(int pid) {
        throw new RuntimeException("Stub!");
    }

    public static final native void sendSignalQuiet(int pid, int signal);

    public static final native long getFreeMemory();

    public static final native long getTotalMemory();

    public static final native void readProcLines(String path, String[] reqFields, long[] outSizes);

    public static final native int[] getPids(String path, int[] lastArray);

    public static final native boolean readProcFile(String file, int[] format, String[] outStrings, long[] outLongs, float[] outFloats);

    public static final native boolean parseProcLine(byte[] buffer, int startIndex, int endIndex, int[] format, String[] outStrings, long[] outLongs, float[] outFloats);

    public static final native int[] getPidsForCommands(String[] cmds);

    public static final native long getPss(int pid);

    public static final native int killProcessGroup(int uid, int pid);

    public static final native void removeAllProcessGroups();
}
