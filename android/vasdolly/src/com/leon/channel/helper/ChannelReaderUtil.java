/*
 * Tencent is pleased to support the open source community by making VasDolly available.
 *
 * Copyright (C) 2018 THL A29 Limited, a Tencent company. All rights reserved.
 *
 * Licensed under the BSD 3-Clause License (the "License");you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 *
 * https://opensource.org/licenses/BSD-3-Clause
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS,WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.leon.channel.helper;

import java.io.File;
import java.nio.ByteBuffer;
import java.util.Map;
import android.content.Context;
import com.leon.channel.reader.ChannelReader;
import com.leon.channel.reader.IdValueReader;

/**
 * Created by leontli on 17/2/12.
 */

public class ChannelReaderUtil {
    public static String getChannel(Context context) {
        String channel = getChannelByV2(context);
        if (channel == null) {
            channel = getChannelByV1(context);
        }

        return channel;
    }

    /**
     * if apk use v2 signature , please use this method to get channel info
     *
     * @param context
     * @return
     */
    public static String getChannelByV2(Context context) {
        String apkPath = context.getApplicationInfo().sourceDir;
        return ChannelReader.getChannelByV2(new File(apkPath));
    }

    /**
     * if apk only use v1 signature , please use this method to get channel info
     *
     * @param context
     * @return
     */
    public static String getChannelByV1(Context context) {
        String apkPath = context.getApplicationInfo().sourceDir;
        return ChannelReader.getChannelByV1(new File(apkPath));
    }


    /**
     * get String value from apk by id in the v2 signature mode
     *
     * @param context
     * @param id
     * @return
     */
    public static String getStringValueById(Context context, int id) {
        String apkPath = context.getApplicationInfo().sourceDir;
        return IdValueReader.getStringValueById(new File(apkPath), id);
    }

    /**
     * get byte[] from apk by id in the v2 signature mode
     *
     * @param context
     * @param id
     * @return
     */
    public static byte[] getByteValueById(Context context, int id) {
        String apkPath = context.getApplicationInfo().sourceDir;
        return IdValueReader.getByteValueById(new File(apkPath), id);
    }

    /**
     * find all Id-Value Pair from Apk in the v2 signature mode
     *
     * @param context
     * @return
     */
    public static Map<Integer, ByteBuffer> getAllIdValueMap(Context context) {
        String apkPath = context.getApplicationInfo().sourceDir;
        return IdValueReader.getAllIdValueMap(new File(apkPath));
    }
}
