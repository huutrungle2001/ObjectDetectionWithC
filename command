gcc cam_detect.c -o cam_detect

./cam_detect s test_calibrations/blobs.txt
./cam_detect s test_calibrations/duck.txt
./cam_detect s test_calibrations/objects.txt
./cam_detect s test_calibrations/redblob.txt
