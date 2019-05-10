package android.support.focus;

import android.content.Context;
import android.graphics.Canvas;
import android.util.AttributeSet;
import android.widget.ImageView;

/**
 * Class <tt>FocusImageView</tt>
 * @author Garfield
 */
public class FocusImageView extends ImageView {
    private final FocusDrawable mDrawable;

    public FocusImageView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public FocusImageView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mDrawable = new FocusDrawable(context, attrs);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        mDrawable.draw(canvas, this, ENABLED_FOCUSED_STATE_SET);
    }
}