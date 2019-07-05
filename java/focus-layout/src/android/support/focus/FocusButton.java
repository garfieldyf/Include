package android.support.focus;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.widget.Button;

/**
 * Class <tt>FocusButton</tt>
 * <p>Note: Don't set android:singleLine="true" in the layout.</p>
 * @author Garfield
 */
public class FocusButton extends Button {
    private final FocusDrawable mDrawable;

    public FocusButton(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FocusButton(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mDrawable = new FocusDrawable(context, attrs);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mDrawable.draw(canvas, this, ENABLED_FOCUSED_STATE_SET);
    }
}
