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
        throw new UnsupportedOperationException();
    }

    public AsyncTask() {
        throw new UnsupportedOperationException();
    }

    public final Status getStatus() {
        throw new UnsupportedOperationException();
    }

    protected abstract Result doInBackground(Params... params);

    protected void onPreExecute() {
    }

    protected void onPostExecute(Result result) {
    }

    protected void onProgressUpdate(Progress... values) {
    }

    protected void onCancelled() {
    }

    public final boolean isCancelled() {
        throw new UnsupportedOperationException();
    }

    public final Result get() throws InterruptedException, ExecutionException {
        throw new UnsupportedOperationException();
    }

    public final Result get(long timeout, TimeUnit unit) throws InterruptedException, ExecutionException, TimeoutException {
        throw new UnsupportedOperationException();
    }

    public final AsyncTask<Params, Progress, Result> execute(Params... params) {
        throw new UnsupportedOperationException();
    }

    public final AsyncTask<Params, Progress, Result> executeOnExecutor(Executor exec, Params... params) {
        throw new UnsupportedOperationException();
    }

    public static void execute(Runnable runnable) {
        throw new UnsupportedOperationException();
    }

    protected final void publishProgress(Progress... values) {
        throw new UnsupportedOperationException();
    }
}
