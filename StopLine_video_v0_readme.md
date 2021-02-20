# Requirement
OpenCV 4.2
gcc -8
# compile
g++ -v StopLine_video_v0.cpp -I/usr/local/include/opencv4 -L/usr/lib/ -lstdc++ -L/usr/local/lib -lopencv_highgui -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_videoio -lopencv_video -lopencv_videostab -o StopLine_video_v0
# execute
./StopLine_video_v0 video.mov
