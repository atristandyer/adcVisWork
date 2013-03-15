#ifndef ADCDATA_H
#define ADCDATA_H

#define EARTH_RADIUS 6378206.4


typedef struct {
		int nodeNumber;
		float x;
		float y;
		float z;
} Node;


typedef struct {
		int elementNumber;
		unsigned int n1;
		unsigned int n2;
		unsigned int n3;
} Element;

#endif // ADCDATA_H
