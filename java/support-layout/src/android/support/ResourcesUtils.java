package android.support;

import android.content.Context;

/**
 * Class <tt>ResourcesUtils</tt>
 * @author Garfield
 */
public final class ResourcesUtils {
    /**
     * Returns the <em>package</em>.R.styleable.<em>name</em> field value.
     * @param context The <tt>Context</tt>.
     * @param name The name of field.
     * @return The value of the <em>name</em> field.
     */
    public static Object getFieldValue(Context context, String name) {
        try {
            return Class.forName(context.getPackageName() + ".R$styleable").getField(name).get(null);
        } catch (Throwable e) {
            throw new AssertionError(e);
        }
    }

    /**
     * This utility class cannot be instantiated.
     */
    private ResourcesUtils() {
    }
}
