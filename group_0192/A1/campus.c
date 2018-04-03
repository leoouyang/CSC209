#include <stdio.h>
#include <stdlib.h>

int main(){
	double latitude;
	double longitude;
	char label[100];
	
	while (fscanf(stdin, "%lf,%lf,%99[^\n]", &latitude, &longitude, label)==3){
		if (latitude < 43.65 || latitude > 43.67)
			fprintf(stderr,"Latitude not in campus\n");
		else if (longitude < -79.41 || longitude > -79.38)
			fprintf(stderr,"Longitude not in campus\n");
		else{
			fprintf(stdout, "%lf,%lf,%s\n",latitude,longitude,label);
		}

	}
	return 0;
}