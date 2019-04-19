package android.os;

import java.util.concurrent.ExecutionException;
import java.util.concurrent.Executor;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

@SuppressWarnings("unchecked")
public abstract class AsyncTask<Params, Progress, Result> {
    public static final Executor THREAD_POOL_EXECUTOR = null;
    public static final Executor SERIAL_EXECUTOR = null;

    public enum Status {
        PENDING, RUNNING, FINISHED,
    }

    public static void setDefaultExecutor(Executor exec) {
        throw new RuntimeException("Stub!");
    }

    public AsyncTask() {
        throw new RuntimeException("Stub!");
    }

    public final Status getStatus() {
        throw new RuntimeException("Stub!");
    }

    protected abstract Result doInBackground(Params... params);

    protected void onPreExecute() {
        throw new RuntimeException("Stub!");
    }

    protected void onPostExecute(Result result) {
        throw new RuntimeException("Stub!");
    }

    protected void onProgressUpdate(Progress... values) {
        throw new RuntimeException("Stub!");
    }

    protected void onCancelled(Result result) {
        throw new RuntimeException("Stub!");
    }

    protected void onCancelled() {
        throw new RuntimeException("Stub!");
    }

    public final boolean isCancelled() {
        throw new RuntimeException("Stub!");
    }

    public final boolean cancel(boolean mayInterruptIfRunning) {
        throw new RuntimeException("Stub!");
    }

    public final Result get() throws InterruptedException, ExecutionException {
        throw new RuntimeException("Stub!");
    }

    public final Result get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
        throw new RuntimeException("Stub!");
    }

    public final AsyncTask<Params, Progress, Result> execute(Params... params) {
        throw new RuntimeException("Stub!");
    }

    public final AsyncTask<Params, Progress, Result> executeOnExecutor(Executor exec, Params... params) {
        throw new RuntimeException("Stub!");
    }

    public static void execute(Runnable runnable) {
        throw new RuntimeException("Stub!");
    }

    protected final void publishProgress(Progress... values) {
        throw new RuntimeException("Stub!");
    }
}
