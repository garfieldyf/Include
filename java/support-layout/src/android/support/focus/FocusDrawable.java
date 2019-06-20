package android.support.focus;

import java.util.concurrent.atomic.AtomicReference;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.DrawableContainer;
import android.util.AttributeSet;
import android.util.StateSet;
import android.view.View;

/**
 * Class <tt>FocusDrawable</tt>
 * @author Garfield
 */
public final class FocusDrawable {
    private static final AtomicReference<Rect> sRectPool = new AtomicReference<Rect>();
    private final Drawable mDrawable;

    /**
     * Constructor
     * @param focus The focus <tt>Drawable</tt>.
     * @see #FocusDrawable(Context, AttributeSet)
     */
    public FocusDrawable(Drawable focus) {
        mDrawable = focus;
    }

    /**
     * Constructor
     * @param context The <tt>Context</tt>.
     * @param attrs The base set of attribute values.
     * @see #FocusDrawable(Drawable)
     */
    public FocusDrawable(Context context, AttributeSet attrs) {
        final TypedArray a = context.obtainStyledAttributes(attrs, (int[])getFieldValue(context, "FocusDrawable"));
        mDrawable = a.getDrawable(0 /* R.styleable.FocusDrawable_foucs */);
        a.recycle();
    }

    /**
     * Draw this focus drawable with the specified the {@link View} states.
     * @param canvas The canvas to draw into.
     * @param view The <tt>View</tt> obtains the current states.
     * @param stateSpec An array of required {@link View} states. If this
     * focus drawable is state full, This parameter will be ignored.
     */
    public void draw(Canvas canvas, View view, int[] stateSpec) {
        final int[] stateSet = view.getDrawableState();
        if (mDrawable.isStateful()) {
            mDrawable.setState(stateSet);
            draw(canvas, view, mDrawable);
        } else if (StateSet.stateSetMatches(stateSpec, stateSet)) {
            draw(canvas, view, mDrawable);
        }
    }

    private void draw(Canvas canvas, View view, Drawable drawable) {
        int left = 0, top = 0, right = view.getWidth(), bottom = view.getHeight();
        if (drawable instanceof DrawableContainer) {
            drawable = ((DrawableContainer)drawable).getCurrent();
        }

        final Rect padding = obtain();
        if (drawable.getPadding(padding)) {
            left   -= padding.left;
            top    -= padding.top;
            right  += padding.right;
            bottom += padding.bottom;
        }

        sRectPool.compareAndSet(null, padding);
        drawable.setBounds(left, top, right, bottom);
        drawable.draw(canvas);
    }

    private static Rect obtain() {
        final Rect result = sRectPool.getAndSet(null);
        return (result != null ? result : new Rect());
    }

    private static Object getFieldValue(Context context, String name) {
        try {
            return Class.forName(context.getPackageName() + ".R$styleable").getField(name).get(null);
        } catch (Throwable e) {
            throw new AssertionError(e);
        }
    }
}
