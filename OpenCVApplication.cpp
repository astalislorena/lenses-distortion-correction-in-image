#include "stdafx.h"
#include "common.h"
#include <vector>
#include <queue>
#include <random>


void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void distorsionElimination() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}


int main()
{
	system("cls");
	destroyAllWindows();
	distorsionElimination();
	return 0;
}