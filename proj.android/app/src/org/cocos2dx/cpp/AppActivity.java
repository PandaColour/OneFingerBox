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

import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.widget.RelativeLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.ads.MobileAds;

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    private static AppActivity _activity;
    private AdView adView;
    private InterstitialAd interstitialAd;

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

        MobileAds.initialize(this, "ca-app-pub-7614285848136573~5029266254");

        adView = new AdView(this);
        adView.setAdUnitId("ca-app-pub-7614285848136573/8202224505");
        adView.setAdSize(AdSize.SMART_BANNER);

        RelativeLayout relativeLayout = new RelativeLayout(_activity);
        mFrameLayout.addView(relativeLayout);

        RelativeLayout.LayoutParams adViewParams = new RelativeLayout.LayoutParams(AdView.LayoutParams.WRAP_CONTENT, AdView.LayoutParams.WRAP_CONTENT);
        adViewParams.addRule(RelativeLayout.ALIGN_PARENT_BOTTOM);
        adViewParams.addRule(RelativeLayout.CENTER_IN_PARENT, RelativeLayout.TRUE);
        relativeLayout.addView(adView, adViewParams);

        adView.setAdListener(new AdListener(){
            @Override
            public void onAdFailedToLoad(int i) {
                super.onAdFailedToLoad(i);
                Log.d("AdView", "AdView onAdFailedToLoad: " + i );
            }

            @Override
            public void onAdLoaded() {
                super.onAdLoaded();
                adView.setBackgroundColor(Color.BLACK);
            }
        });


        interstitialAd = new InterstitialAd(this);
        interstitialAd.setAdUnitId("ca-app-pub-7614285848136573/5881869843");
        interstitialAd.setAdListener(new AdListener() {
            @Override
            public void onAdFailedToLoad(int i) {
                super.onAdFailedToLoad(i);
                Log.d("InterstitialAd", "InterstitialAd onAdFailedToLoad: " + i );
            }

            @Override
            public void onAdOpened() {
                super.onAdOpened();
            }

            @Override
            public void onAdClosed() {
                super.onAdClosed();
            }
        });



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

    public static void showBanner(){
        _activity._showBanner();
    }

    public void _showBanner() {
        if (adView != null) {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    AdRequest adRequest = new AdRequest.Builder().addTestDevice("00721F2E4EF2EE0AC844D815C3A035A3").build();
                    _activity.adView.loadAd(adRequest);
                }
            });
        }
    }

    public static void showFullAd(){
        _activity._showFullAd();
    }

    public void _showFullAd() {
        if (adView != null) {
            _activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    if (_activity.interstitialAd != null && _activity.interstitialAd.isLoaded()) {
                        _activity.interstitialAd.show();
                    }

                    if (!interstitialAd.isLoading() && !interstitialAd.isLoaded()) {
                        AdRequest adRequest = new AdRequest.Builder().addTestDevice("00721F2E4EF2EE0AC844D815C3A035A3").build();
                        _activity.interstitialAd.loadAd(adRequest);
                    }
                }
            });
        }
    }


}