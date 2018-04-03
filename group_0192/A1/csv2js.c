#include <stdio.h>
#include <stdlib.h>

int main(){
	double latitude;
	double longitude;
	char label[100];
	
	int started = 0;
	fprintf(stdout, "data=[\n");
	
	while (fscanf(stdin, "%lf,%lf,%99[^\n]", &latitude, &longitude, label)==3){
		if (latitude < -90 || latitude > 90)
			fprintf(stderr,"Invalid latitude\n");
		else if (longitude < -180 || longitude > 180)
			fprintf(stderr,"Invalid longtitude\n");
		else{
			if(started)
				fprintf(stdout, ",\n");
			else
				started = 1;
			
			fprintf(stdout, "{latitude: %lf, longitude: %lf, label: '%s'}",latitude,longitude,label);
		}

	}
	
	fprintf(stdout, "\n];");
	return 0;




}