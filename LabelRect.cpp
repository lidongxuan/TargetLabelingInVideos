#include <stdio.h>
#include <iostream>
#include <cv.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat frame;
Rect select;
bool select_flag = false;
bool first_select_flag = false;
Point start_point;
Point end_point;

void mouse_event(int event, int x, int y, int, void*);

int main()
{
	VideoCapture orig_video;
	orig_video.open("1.mp4");//����·��
	namedWindow("window", 1);
	Mat frame_show;
	//track tracker;
	cvSetMouseCallback("window", mouse_event, 0);//ע�������Ӧ�ص�����

	int frame_num = 0;

	while (orig_video.read(frame))
	{
		while (1)
		{
			frame.copyTo(frame_show);
			rectangle(frame_show, select, Scalar(255, 0, 0), 2, 8, 0);//�ܹ�ʵʱ��ʾ�ڻ����δ���ʱ�ĺۼ�
			imshow("window", frame_show);
			waitKey(10);
			if (select_flag == false && first_select_flag == true)
			{
				first_select_flag = false;
				select.x = 0;
				select.y = 0;
				select.width = 0;
				select.height = 0;
				if (waitKey(0) == 13)//ѡ����»س���ȷ����Ч���������������¿�ѡ
				{
					break;
				}
			}
		}

		frame_num++;
		//Ŀ���rect��Ϣ����ʾ���������
		cout << start_point.x << "," << start_point.y << "," << end_point.x - start_point.x << "," << end_point.y - start_point.y << endl;
		string s;
		char frame_num_str[64];
		sprintf(frame_num_str, "%d", frame_num + 10000);
		s = frame_num_str;
		//ͼƬ�ᱣ���ڳ���Ŀ¼��img���ļ�����
		imwrite("./img/" + s.substr(1, 4) + ".jpg",frame);

	}
}

void mouse_event(int event, int x, int y, int, void*)
{
	//Point start_point;//����������ط����ж��壬��Ϊ���ǻ�����Ϣ��Ӧ�ĺ�����ִ�����start_point���ͷ��ˣ����Դﲻ��Ч����
	if (select_flag)
	{
		select.x = MIN(start_point.x, x);//��һ��Ҫ����굯��ż�����ο򣬶�Ӧ������갴�¿�ʼ���������ʱ��ʵʱ������ѡ���ο�
		select.y = MIN(start_point.y, y);
		select.width = abs(x - start_point.x);//����ο�Ⱥ͸߶�
		select.height = abs(y - start_point.y);
		select &= Rect(0, 0, frame.cols, frame.rows);//��֤��ѡ���ο�����Ƶ��ʾ����֮��
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		select_flag = true;//��갴�µı�־����ֵ
		first_select_flag = true;
		start_point = Point(x, y);//�������������ǲ�׽���ĵ�
		select = Rect(x, y, 0, 0);//����һ��Ҫ��ʼ������͸�Ϊ(0,0)����Ϊ��opencv��Rect���ο����ڵĵ��ǰ������Ͻ��Ǹ���ģ����ǲ������½��Ǹ���
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		end_point = Point(x, y);
		select_flag = false;
	}
}