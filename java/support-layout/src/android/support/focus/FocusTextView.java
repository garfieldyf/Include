package android.support.focus;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.widget.TextView;

/**
 * Class <tt>FocusTextView</tt>
 * <p>Note: Don't set android:singleLine="true" in the layout.</p>
 * @author Garfield
 */
public class FocusTextView extends TextView {
    private final FocusDrawable mDrawable;

    public FocusTextView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FocusTextView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mDrawable = new FocusDrawable(context, attrs);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mDrawable.draw(canvas, this, ENABLED_FOCUSED_STATE_SET);
    }
}
