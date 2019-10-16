package com.example.bleapp;

import android.content.Context;
import android.support.v4.app.Fragment;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.jjoe64.graphview.GraphView;
import com.jjoe64.graphview.series.DataPoint;
import com.jjoe64.graphview.series.LineGraphSeries;

public class RealtimeUpdates extends Fragment {
    //private final Handler mHandler = new Handler();
    private LineGraphSeries<DataPoint> mSeries1;
    //private Runnable mTimer1;
    float samples[] = new float[250];
    static float plotData[] = new float[1000];
    public static int currentTimeSec = 0;
    public static int min = 0;
    public static int max = 1000;
    public static int flag = 0;
    float in[] = new float[3];
    float fb[] = new float[2];

    public native void channelFilter(float[] input, float[] _in, float[] _fb);

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {

        View rootView = inflater.inflate(R.layout.graph_viewer, container, false);
        GraphView graph = (GraphView) rootView.findViewById(R.id.graph);
        mSeries1 = new LineGraphSeries<>();

        //setting the graph bounds
        graph.getViewport().setXAxisBoundsManual(true);
        graph.getViewport().setMinX(min);
        graph.getViewport().setMaxX(max);
        /*graph.getViewport().setYAxisBoundsManual(true);
        graph.getViewport().setMinY(-2.5);
        graph.getViewport().setMaxY(2.5);*/

        //enable scaling and scrolling
        graph.getViewport().setScrollable(true);
        graph.getViewport().setScrollableY(true);
        graph.getViewport().setScalable(true);
        graph.getViewport().setScalableY(true);

        //other parameters
        graph.setKeepScreenOn(true);
        graph.getGridLabelRenderer().setHorizontalAxisTitle("Time");
        graph.getGridLabelRenderer().setVerticalAxisTitle("mV");
        mSeries1.setColor(Color.BLUE);

        //Get UartService data and append them on the graph
        if(flag==1){
            for (int i = 10; i < 250; i++)
                mSeries1.appendData(new DataPoint(i, plotData[i]), false, 249);//1st 250 samples - 1 sec
        }else if(flag==2){
            for (int i = 10; i < 500; i++) {
                if (i % 250 == 0)
                    i = i + 10;
                mSeries1.appendData(new DataPoint(i, plotData[i]), false, 499);//1st 500 samples - 2 secs
            }
        }else if(flag==3){
            for (int i = 10; i < 750; i++) {
                if (i % 250 == 0)
                    i = i + 10;
                mSeries1.appendData(new DataPoint(i, plotData[i]), false, 749);//1st 750 samples - 3 secs
            }
        }else if(flag==4){
            for (int i = 10; i < 1000; i++) {
                if (i % 250 == 0)
                    i = i + 10;
                mSeries1.appendData(new DataPoint(i, plotData[i]), false, 999);// 1st 1000 samples - 4 secs
            }
        }else{
            for (int i = 10; i < 1000; i++) {
                if (i % 250 == 0)
                    i = i + 10;
                mSeries1.appendData(new DataPoint(min + i, plotData[i]), false, 1000);//refresh to graph the last 1000 samples - 4 last secs
            }
        }

        currentTimeSec = currentTimeSec + 250;

        if (currentTimeSec == max) {
            min = min + 250;
            max = max + 250;
        }

        graph.addSeries(mSeries1);

        return rootView;
    }

    @Override
    public void onAttach(Context context){
        super.onAttach(context);
        for (int i = 0; i < 250; i++) {
            samples[i] = ((MainActivity) getActivity()).data[i];
        }

        //dataFiltering();
        //createPlotArray(samples);
        channelFilter(samples, in, fb);
        createPlotArray(samples);
    }

    public void dataFiltering(){

    }

    public void createPlotArray(float[] temp){
        if(flag<4){
            for (int i = 0; i < 250; i++)
                plotData[flag*250+i] = temp[i];
        }
        else{
            for (int i = 0; i < 750; i++)
                plotData[i] = plotData[i+250];

            for (int i = 0; i < 250; i++)
                plotData[i+750] = temp[i];
        }
        flag++;
    }
}