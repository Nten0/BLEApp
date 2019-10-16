/* Copyright (C) 2017 Theodoros Foradis */

/* This program is free software; you can redistribute it and/or */
/* modify it under the terms of the GNU General Public License as */
/* published by the Free Software Foundation; either version 3 of */
/* the License, or (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, but */
/* WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU */
/* General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <stdio.h>
#include <string.h>
#include <jni.h>

    /*JNIEXPORT void JNICALL
    Java_com_example_bleapp_RealtimeUpdates_floatFromJNI(JNIEnv *env, jobject instance, jfloatArray samples) {
        jfloat *local = (*env)->GetFloatArrayElements(env, samples, NULL);

        for(int i=0; i<250; i++){
            local[i] = i;
        }

        (*env)->ReleaseFloatArrayElements(env, samples, local, 0);
    }*/


static const float a1[3]={1.00000,  -0.50000,  -0.50000};
static const float b1[3]={0.25000,  -0.50000,   0.25000};
static const float a2[3]={1.00000,  -1.58770,   0.96251};
static const float b2[3]={0.98126,  -1.58770,   0.98126};
static const float a3[3]={1.00000,  -0.61803,   1.00000};
static const float b3[3]={1.00000,  -0.61803,   1.00000};
static const float a4[3]={1.00000,   0.59733,   0.86116};
static const float b4[3]={0.93058,   0.59733,   0.93058};
static const float a5[3]={1.00000,   1.52563,   0.86886};
static const float b5[3]={0.93443,   1.52563,   0.93443};
static const float G[5]={0.1, 0.06, 0.006, 0.07, 0.0006};

float filter(float *yin, float *yfb)
{

    // discrete equation
    //
    //a1=      1.00000  -0.50000  -0.50000
    //b1=      0.25000  -0.50000   0.25000
    //a2=     1.00000  -1.58770   0.96251
    //b2=      0.98126  -1.58770   0.98126
    //a3=     1.00000  -0.61803   1.00000
    //b3=     1.00000  -0.61803   1.00000
    //a4 =    1.00000   0.59733   0.86116
    //b4 =    0.93058   0.59733   0.93058
    //a5 =    1.00000   1.52563   0.86886
    //b5 =    0.93443   1.52563   0.93443
    //G=   1.3 0.04 0.01 0.19 0.125

    float ydc=(b1[0]*yin[0]+b1[1]*yin[1]+b1[2]*yin[2])-(a1[0]*yfb[0]+a1[1]*yfb[1]);
    float y50=(b2[0]*yin[0]+b2[1]*yin[1]+b2[2]*yin[2])-(a2[0]*yfb[0]+a2[1]*yfb[1]);
    float y100=(b3[0]*yin[0]+b3[1]*yin[1]+b3[2]*yin[2])-(a3[0]*yfb[0]+a3[1]*yfb[1]);
    float y150= (b4[0]*yin[0]+b4[1]*yin[1]+b4[2]*yin[2])-(a4[0]*yfb[0]+a4[1]*yfb[1]);
    float y200= (b5[0]*yin[0]+b5[1]*yin[1]+b5[2]*yin[2])-(a5[0]*yfb[0]+a5[1]*yfb[1]);

    // final value compute with gains
    return G[0]*ydc+G[1]*y50+G[2]*y100+G[3]*y150+G[4]*y200;
}


void channel_filter_three(float* in, float* fb, float *out){
    // filter channel
    *out=filter(in,fb);
    //input shift
    in[2]=in[1];
    in[1]=in[0];
    //output shift
    fb[1]=fb[0];
    fb[0]=*out;

    return;
}

JNIEXPORT void JNICALL
Java_com_example_bleapp_RealtimeUpdates_channelFilter(JNIEnv *env, jobject instance,
                                                       jfloatArray _input, jfloatArray _in, jfloatArray _fb){
    jfloat* input  = (*env)->GetFloatArrayElements(env, _input, NULL);
    jfloat* in  = (*env)->GetFloatArrayElements(env, _in, NULL);
    jfloat* fb  = (*env)->GetFloatArrayElements(env, _fb, NULL);

    float out[250];
    memset(out, 0, sizeof(out));

    for(int i=0; i<250; i++) {
        in[0]=input[i];
        channel_filter_three(in,fb,&out[i]);
    }

    memcpy(input, out, sizeof(out));

    (*env)->ReleaseFloatArrayElements(env, _input, input, 0);
    (*env)->ReleaseFloatArrayElements(env, _in, in, 0);
    (*env)->ReleaseFloatArrayElements(env, _fb, fb, 0);

    return;

}