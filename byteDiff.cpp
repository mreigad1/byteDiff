#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int main() {
	int height;
	int width;
	string image1;
	string image2;

	cin >> height;
	cin >> width;

	const int imageSize = height * width;

	if (imageSize <= 0) {
		cout << "Invalid Height or Width.\n";
		return -1;
	}

	cin >> image1;
	cin >> image2;

	int fd[2] = { 0,0 };
	unsigned char* imgMap[2] = { NULL, NULL }; 

	fd[0] = open(image1.c_str(), O_RDWR);
	fd[1] = open(image2.c_str(), O_RDWR);

	if (0 == fd[0] || 0 ==fd[1]) {
		cout << "failure opening file handles";
		return -2;
	}

	imgMap[0] = (unsigned char*) mmap(NULL, imageSize, PROT_READ, MAP_SHARED, fd[0], 0);
	imgMap[1] = (unsigned char*) mmap(NULL, imageSize, PROT_READ, MAP_SHARED, fd[1], 0);


	if (0 == imgMap[0]) {
		cout << "failure mmaping files " << __LINE__ << endl;
		return -5;

	} else if (0 == imgMap[1]) {
		cout << "failure mmaping files " << __LINE__ << endl;
		return -6;

	} else if (MAP_FAILED == imgMap[0]) {
		cout << "failure mmaping files " << __LINE__ << endl;
		return -7;

	} else if (MAP_FAILED == imgMap[1]) {
		cout << "failure mmaping files " << __LINE__ << endl;
		return -8;
	}

	int countDiff = 0;
	const int diffThresh = 10;
	int k = 0;
	int diff = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			k = i * width + j;
			diff = imgMap[0][k];
			diff -= imgMap[1][k];
			diff = (diff < 0) ? (diff * -1) : diff;

			if (diff >= diffThresh) {
				countDiff++;
			}
		}
	}

	munmap(imgMap[0], imageSize);
	munmap(imgMap[1], imageSize);
	close(fd[0]);
	close(fd[1]);

	double totalDifference = countDiff;
		totalDifference /= imageSize;

	cout << totalDifference << endl;

	return 0;
}