#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "TutorialConfig.h"
int main (int argc, char *argv[]){
	if (argc < 2){
		fprintf(stdout,"%s Version %d.%d\n",
            argv[0],
            Tutorial_VERSION_MAJOR,
            Tutorial_VERSION_MINOR);
		fprintf(stdout,"Usage: %s number\n",argv[0]);
		return 1;
    }
	double inputValue = atof(argv[1]);
	double outputValue = sqrt(inputValue);
	fprintf(stdout,"The square root of %g is %g\n",inputValue, outputValue);
	return 0;
}


//作者：saka
//链接：https://juejin.im/post/5a6f32e86fb9a01ca6031230
//来源：掘金
//著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。