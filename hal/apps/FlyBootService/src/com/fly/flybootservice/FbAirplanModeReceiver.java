/*
 * Copyright (c) 2012, The Linux Foundation. All rights reserved.

 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *  * Neither the name of The Linux Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

package com.fly.flybootservice;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import java.io.File;
import java.io.FileOutputStream;

public class FbAirplanModeReceiver extends BroadcastReceiver {
    public final static String LOG_TAG = "FbAirplanModeReceiver";
    @Override
    public void onReceive(Context context, Intent intent) {
       if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
		LIDBG_PRINT("receive Intent.ACTION_BOOT_COMPLETED");
            try {
                Context flyBootServiceContext = context.createPackageContext("com.fly.flybootservice",
                    Context.CONTEXT_INCLUDE_CODE | Context.CONTEXT_IGNORE_SECURITY);
                if(flyBootServiceContext != null) {
                    Class FlyBootService = flyBootServiceContext.getClassLoader().loadClass("com.fly.flybootservice.FlyBootService");
                    if (FlyBootService != null) {
                        Object flyBootServiceObj = FlyBootService.newInstance();
                        FlyBootService.getMethod("restoreAirplaneMode", Context.class).invoke(flyBootServiceObj, flyBootServiceContext);
                    } else {
                        Log.e(LOG_TAG, "can`t get the class of FlyBootService");
                    }
                } else {
                    Log.e(LOG_TAG, "can`t get the context of FlyBootService");
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

 private static void LIDBG_PRINT(String msg) {
        Log.d(LOG_TAG, msg);
        String newmsg = "lidbg_msg: " + msg;
        File mFile = new File("/dev/lidbg_msg");
        if (mFile.exists()) {
            try {
                FileOutputStream fout = new FileOutputStream(
                        mFile.getAbsolutePath());
                byte[] bytes = newmsg.getBytes();
                fout.write(bytes);
                fout.close();
            } catch (Exception e) {
                Log.e(LOG_TAG, "Failed to lidbg_printk");
            }

        } else {
            Log.e(LOG_TAG, "/dev/lidbg_msg not exist");
        }
    }
}

