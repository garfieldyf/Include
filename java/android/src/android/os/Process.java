package android.os;

public class Process {
    public static final String ZYGOTE_SOCKET = "zygote";
    public static final String SECONDARY_ZYGOTE_SOCKET = "zygote_secondary";

    public static final int ROOT_UID = 0;
    public static final int SYSTEM_UID = 1000;
    public static final int PHONE_UID = 1001;
    public static final int SHELL_UID = 2000;
    public static final int LOG_UID = 1007;
    public static final int WIFI_UID = 1010;
    public static final int MEDIA_UID = 1013;
    public static final int DRM_UID = 1019;
    public static final int VPN_UID = 1016;
    public static final int KEYSTORE_UID = 1017;
    public static final int NFC_UID = 1027;
    public static final int BLUETOOTH_UID = 1002;
    public static final int MEDIA_RW_GID = 1023;
    public static final int PACKAGE_INFO_GID = 1032;
    public static final int SHARED_RELRO_UID = 1037;
    public static final int AUDIOSERVER_UID = 1041;
    public static final int CAMERASERVER_UID = 1047;
    public static final int WEBVIEW_ZYGOTE_UID = 1051;
    public static final int OTA_UPDATE_UID = 1061;
    public static final int FIRST_APPLICATION_UID = 10000;
    public static final int LAST_APPLICATION_UID = 19999;
    public static final int FIRST_ISOLATED_UID = 99000;
    public static final int LAST_ISOLATED_UID = 99999;
    public static final int SHARED_USER_GID = 9997;
    public static final int FIRST_SHARED_APPLICATION_GID = 50000;
    public static final int LAST_SHARED_APPLICATION_GID = 59999;
    public static final int FIRST_APPLICATION_CACHE_GID = 20000;
    public static final int LAST_APPLICATION_CACHE_GID = 29999;

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

    public static final int SCHED_OTHER = 0;
    public static final int SCHED_FIFO = 1;
    public static final int SCHED_RR = 2;
    public static final int SCHED_BATCH = 3;
    public static final int SCHED_IDLE = 5;

    public static final int THREAD_GROUP_DEFAULT = -1;
    public static final int THREAD_GROUP_BG_NONINTERACTIVE = 0;
    public static final int THREAD_GROUP_SYSTEM = 2;
    public static final int THREAD_GROUP_AUDIO_APP = 3;
    public static final int THREAD_GROUP_AUDIO_SYS = 4;
    public static final int THREAD_GROUP_TOP_APP = 5;

    public static final int SIGNAL_QUIT = 3;
    public static final int SIGNAL_KILL = 9;
    public static final int SIGNAL_USR1 = 10;

    public static final ProcessStartResult start(final String processClass,
                                                 final String niceName,
                                                 int uid, int gid, int[] gids,
                                                 int debugFlags, int mountExternal,
                                                 int targetSdkVersion,
                                                 String seInfo,
                                                 String abi,
                                                 String instructionSet,
                                                 String appDataDir,
                                                 String invokeWith,
                                                 String[] zygoteArgs) {
        throw new RuntimeException("Stub!");
    }

    public static final ProcessStartResult startWebView(final String processClass,
                                                        final String niceName,
                                                        int uid, int gid, int[] gids,
                                                        int debugFlags, int mountExternal,
                                                        int targetSdkVersion,
                                                        String seInfo,
                                                        String abi,
                                                        String instructionSet,
                                                        String appDataDir,
                                                        String invokeWith,
                                                        String[] zygoteArgs) {
        throw new RuntimeException("Stub!");
    }

    public static final native long getElapsedCpuTime();

    public static final long getStartElapsedRealtime() {
        throw new RuntimeException("Stub!");
    }

    public static final long getStartUptimeMillis() {
        throw new RuntimeException("Stub!");
    }

    public static final void setStartTimes(long elapsedRealtime, long uptimeMillis) {
        throw new RuntimeException("Stub!");
    }

    public static final boolean is64Bit() {
        throw new RuntimeException("Stub!");
    }

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

    public static UserHandle myUserHandle() {
        throw new RuntimeException("Stub!");
    }

    public static boolean isApplicationUid(int uid) {
        throw new RuntimeException("Stub!");
    }

    public static final boolean isIsolated() {
        throw new RuntimeException("Stub!");
    }

    public static final boolean isIsolated(int uid) {
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

    public static final native void setThreadPriority(int tid, int priority)
        throws IllegalArgumentException, SecurityException;

    public static final native void setCanSelfBackground(boolean backgroundOk);

    public static final native void setThreadGroup(int tid, int group)
        throws IllegalArgumentException, SecurityException;

    public static final native void setThreadGroupAndCpuset(int tid, int group)
        throws IllegalArgumentException, SecurityException;

    public static final native void setProcessGroup(int pid, int group)
        throws IllegalArgumentException, SecurityException;

    public static final native int getProcessGroup(int pid)
        throws IllegalArgumentException, SecurityException;

    public static final native int[] getExclusiveCores();

    public static final native void setThreadPriority(int priority)
        throws IllegalArgumentException, SecurityException;

    public static final native int getThreadPriority(int tid)
        throws IllegalArgumentException;

    public static final native int getThreadScheduler(int tid)
        throws IllegalArgumentException;

    public static final native void setThreadScheduler(int tid, int policy, int priority)
        throws IllegalArgumentException;

    @Deprecated
    public static final boolean supportsProcesses() {
        throw new RuntimeException("Stub!");
    }

    public static final native boolean setSwappiness(int pid, boolean is_increased);

    public static final native void setArgV0(String text);

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

    public static final int PROC_TERM_MASK = 0xff;
    public static final int PROC_ZERO_TERM = 0;
    public static final int PROC_SPACE_TERM = (int)' ';
    public static final int PROC_TAB_TERM = (int)'\t';
    public static final int PROC_COMBINE = 0x100;
    public static final int PROC_PARENS = 0x200;
    public static final int PROC_QUOTES = 0x400;
    public static final int PROC_CHAR = 0x800;
    public static final int PROC_OUT_STRING = 0x1000;
    public static final int PROC_OUT_LONG = 0x2000;
    public static final int PROC_OUT_FLOAT = 0x4000;

    public static final native boolean readProcFile(String file, int[] format, String[] outStrings, long[] outLongs, float[] outFloats);

    public static final native boolean parseProcLine(byte[] buffer, int startIndex, int endIndex, int[] format, String[] outStrings, long[] outLongs, float[] outFloats);

    public static final native int[] getPidsForCommands(String[] cmds);

    public static final native long getPss(int pid);

    public static final class ProcessStartResult {
        public int pid;
        public boolean usingWrapper;
    }

    public static final native int killProcessGroup(int uid, int pid);

    public static final native void removeAllProcessGroups();

    public static final boolean isThreadInProcess(int tid, int pid) {
        throw new RuntimeException("Stub!");
    }
}
