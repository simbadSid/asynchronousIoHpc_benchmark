#include <stdio.h>
#include <stdio.h>

#include <omp.h>

#include <unistd.h>



int main()
{
	#pragma omp parallel num_threads(5)
	{
		#pragma omp for
		for (int i=0; i<200; i++)
		{
			printf ("Iteration :%d: %d\n", omp_get_thread_num(), i);
			fflush(stdout);
		}
	}

	printf("All done\n");
	return 0;
}
