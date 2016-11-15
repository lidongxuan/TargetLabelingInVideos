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
	orig_video.open("1.mp4");//视屏路径
	namedWindow("window", 1);
	Mat frame_show;
	//track tracker;
	cvSetMouseCallback("window", mouse_event, 0);//注册鼠标响应回调函数

	int frame_num = 0;

	while (orig_video.read(frame))
	{
		while (1)
		{
			frame.copyTo(frame_show);
			rectangle(frame_show, select, Scalar(255, 0, 0), 2, 8, 0);//能够实时显示在画矩形窗口时的痕迹
			imshow("window", frame_show);
			waitKey(10);
			if (select_flag == false && first_select_flag == true)
			{
				first_select_flag = false;
				select.x = 0;
				select.y = 0;
				select.width = 0;
				select.height = 0;
				if (waitKey(0) == 13)//选完后按下回车键确认有效，按其他键则重新框选
				{
					break;
				}
			}
		}

		frame_num++;
		//目标的rect信息会显示到命令窗口里
		cout << start_point.x << "," << start_point.y << "," << end_point.x - start_point.x << "," << end_point.y - start_point.y << endl;
		string s;
		char frame_num_str[64];
		sprintf(frame_num_str, "%d", frame_num + 10000);
		s = frame_num_str;
		//图片会保存在程序目录的img子文件夹中
		imwrite("./img/" + s.substr(1, 4) + ".jpg",frame);

	}
}

void mouse_event(int event, int x, int y, int, void*)
{
	//Point start_point;//不能在这个地方进行定义，因为这是基于消息响应的函数，执行完后start_point就释放了，所以达不到效果。
	if (select_flag)
	{
		select.x = MIN(start_point.x, x);//不一定要等鼠标弹起才计算矩形框，而应该在鼠标按下开始到弹起这段时间实时计算所选矩形框
		select.y = MIN(start_point.y, y);
		select.width = abs(x - start_point.x);//算矩形宽度和高度
		select.height = abs(y - start_point.y);
		select &= Rect(0, 0, frame.cols, frame.rows);//保证所选矩形框在视频显示区域之内
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		select_flag = true;//鼠标按下的标志赋真值
		first_select_flag = true;
		start_point = Point(x, y);//保存下来单击是捕捉到的点
		select = Rect(x, y, 0, 0);//这里一定要初始化，宽和高为(0,0)是因为在opencv中Rect矩形框类内的点是包含左上角那个点的，但是不含右下角那个点
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		end_point = Point(x, y);
		select_flag = false;
	}
}