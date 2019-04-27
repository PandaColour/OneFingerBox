/****************************************************************************
Copyright (c) 2015-2016 Chukong Technologies Inc.
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package org.cocos2dx.cpp;

import android.os.Bundle;
import android.view.Gravity;
import android.widget.LinearLayout;
import android.widget.PopupWindow;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewGroup.MarginLayoutParams;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.MobileAds;

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    private static AppActivity _activity;
    private AdView adView;

    private PopupWindow popUp;
    private LinearLayout layout;
    private LinearLayout mainLayout;
    boolean adsinited = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.setEnableVirtualButton(false);
        super.onCreate(savedInstanceState);
        // Workaround in https://stackoverflow.com/questions/16283079/re-launch-of-activity-on-home-button-but-only-the-first-time/16447508
        if (!isTaskRoot()) {
            // Android launched another instance of the root activity into an existing task
            //  so just quietly finish and go away, dropping the user back into the activity
            //  at the top of the stack (ie: the last state of this task)
            // Don't need to finish it again since it's finished in super.onCreate .
            return;
        }

        _activity = this;

        //getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        MobileAds.initialize(this, "ca-app-pub-7614285848136573~2492609446");

        adView = new AdView(this);
        adView.setAdUnitId("ca-app-pub-7614285848136573/3969342649");
        adView.setAdSize(AdSize.SMART_BANNER);

//        RelativeLayout relativeLayout = new RelativeLayout(this);
//        mFrameLayout.addView(relativeLayout);
//
//        RelativeLayout.LayoutParams adViewParams = new RelativeLayout.LayoutParams(AdView.LayoutParams.WRAP_CONTENT, AdView.LayoutParams.WRAP_CONTENT);
//        adViewParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
//        adViewParams.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);
//        relativeLayout.addView(adView, adViewParams);
//
//        AdRequest.Builder adRequestBuilder = new AdRequest.Builder();
//        adRequestBuilder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
//        adView.loadAd(adRequestBuilder.build());
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        adView.resume();
    }

    @Override
    protected void onDestroy() {
        // TODO Auto-generated method stub
        super.onDestroy();
        adView.destroy();
    }

    public static void showAdPopup(){
        _activity._showAdPopup();
    }

    public void _showAdPopup() {
        if (adsinited) {
            return;
        }
        if (adView != null) {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    adsinited = true;

                    popUp = new PopupWindow(_activity);

                    int screenWidth = _activity.getWindowManager()
                            .getDefaultDisplay().getHeight();
                    popUp.setWidth(screenWidth);
                    popUp.setHeight(50);

                    popUp.setWindowLayoutMode(LayoutParams.WRAP_CONTENT,
                            LayoutParams.WRAP_CONTENT);
                    popUp.setClippingEnabled(false);
                    LinearLayout layout = new LinearLayout(_activity);
                    mainLayout = new LinearLayout(_activity);
                    // The layout system for the PopupWindow will kill some
                    // pixels due to margins/paddings etc… (No way to remove
                    // it), so padd it to adjust
                    layout.setPadding(0, 0, 0, 0);
                    MarginLayoutParams params = new MarginLayoutParams(
                            LayoutParams.FILL_PARENT,
                            LayoutParams.FILL_PARENT);
                    params.setMargins(0, 0, 0, 0);
                    layout.setOrientation(LinearLayout.VERTICAL);
                    popUp.setContentView(layout);
//                    _activity.setContentView(mainLayout, params);

                    mFrameLayout.addView(mainLayout);
                    mainLayout.addView(adView);
                    AdRequest adRequest = new AdRequest.Builder().build();
                    _activity.adView.loadAd(adRequest);
                    // Show our popup window
                    popUp.showAtLocation(mainLayout, Gravity.BOTTOM, 0, 0);
                    popUp.update();
                }
            });
        }
    }

}