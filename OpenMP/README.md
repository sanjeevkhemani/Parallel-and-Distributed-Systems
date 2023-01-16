I have run the program with N = 1000 (below) and 2048 (for tasks to match the question prompt’s serial implementation). I will be using the latter value, 2048, for tasks 1 & 2. I will be averaging out the values over 3 runs to compare the three implementations and measure the improvement.

![image](https://user-images.githubusercontent.com/54830217/212600751-20a1ab16-6b67-4232-824f-cbdccd613da9.png)

OpenMP parallel, blocks and sequential calculation time for N = 1000

Task I:
Comparison between sequential and parallel execution:
N = 2048	          Sequential	  Parallel
First Run	          116.342	      11.727
Second Run	        116.541	      12.024
Third Run	          113.859	      11.847
Average Run Time	  115.581	      11.866

Sequential Average Time = 115.581 seconds
Parallel Average Time = 11.866 seconds
Improvement = (11.866 / 115.581) x 100 = 10.266%
Runtime reduced by 89.744%
Verifying with N=1000 solitary run,
Improvement = (0.467 / 3.216) x 100 = 14.521%
Runtime reduced by 85.479%


![image](https://user-images.githubusercontent.com/54830217/212600706-450526ff-b040-4a79-96bc-dbf66aef36ce.png)

Sequential calculation time


![image](https://user-images.githubusercontent.com/54830217/212600670-c8a690e1-6096-4dce-a4c8-ad6744f55949.png)

OpenMP parallel calculation time

Task II:
Comparison between parallel and blocks execution:
N = 2048	          Parallel	    Blocks
First Run	          11.727	      5.501
Second Run	        12.024	      5.435
Third Run	          11.847	      5.401
Average Run Time	  11.866	      5.446

Parallel Average Time = 11.866 seconds
Blocks Average Time = 5.446 seconds
Improvement = (5.446 / 11.866) x 100 = 45.896%
Runtime further reduced by 54.104%
Verifying with solitary N=1000 run,
Improvement = (0.331 / 0.467) x 100 = 70.878%
Runtime reduced by 29.122%


![image](https://user-images.githubusercontent.com/54830217/212600616-3f4dc4f9-8454-4062-ad3b-ec1f023d48de.png)

OpenMP parallel blocks calculation time
