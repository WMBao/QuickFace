// Project: QuickFace 
// Software Engineering Course Project(Autumn 2016)
// G26 - Bo Pang, Weiming Bao & Zifan Liu
// v 1.0.0 - 2017.1.6

#include "helpers.hpp"

#include "eos/core/Landmark.hpp"
#include "eos/core/LandmarkMapper.hpp"
#include "eos/fitting/fitting.hpp"
#include "eos/fitting/orthographic_camera_estimation_linear.hpp"
#include "eos/fitting/contour_correspondence.hpp"
#include "eos/fitting/closest_edge_fitting.hpp"
#include "eos/fitting/RenderingParameters.hpp"
#include "eos/render/utils.hpp"
#include "eos/render/render.hpp"
#include "eos/render/texture_extraction.hpp"


//////////////////////////////////////////////////
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <errno.h>  
#include <unistd.h>  
#include <sys/shm.h>  
#include <sys/types.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
///////////////////////////////////////////////////


#include "rcr/model.hpp"
#include "cereal/cereal.hpp"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Eigen/Dense"

#include "opencv2/core/core.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"


////////////////////////////////////////////////////
#include "opencv2/opencv.hpp"  
///////////////////////////////////////////////////


#include "boost/program_options.hpp"
#include "boost/filesystem.hpp"

#include <vector>
#include <iostream>

using namespace eos;

using namespace cv;  

#define PACKAGE_NUM 32  
  
#define IMG_WIDTH 640  
#define IMG_HEIGHT 480  
  
#define BLOCKSIZE IMG_WIDTH*IMG_HEIGHT*3/PACKAGE_NUM  
#define FILE_BUFFER_SIZE 102400
#define STR_BUFFER_SIZE 256	


namespace po = boost::program_options;
namespace fs = boost::filesystem;
using cv::Mat;
using cv::Vec2f;
using cv::Vec3f;
using cv::Vec4f;
using cv::Rect;
using std::cout;
using std::endl;
using std::vector;
using std::string;

void draw_axes_topright(float r_x, float r_y, float r_z, cv::Mat image);

struct sentbuf{
	char buf[BLOCKSIZE*4/3];
	int flag;
};

struct recvBuf  
{  
    char buf[BLOCKSIZE];  
    int flag;  
};  
  

class SocketLogTransmissionServer  
{  
public:  
    SocketLogTransmissionServer(void);  
    ~SocketLogTransmissionServer(void);  
    //int sockConn;  //socket
    int client_sockfd;
    int server_sockfd;
private:  
   //  struct recvBuf data1;  
  	// struct sentbuf data2;
   //  int needRecv;  
   //  int count;  
	timeval tv = {2, 0};
	fd_set fd;
	int maxsock = 0;
  
public:  
  
    int socketConnect(int PORT);  

    char* receive(char* str);  
  
	int transmit(char* str);
 
    void socketDisconnect(void);  
};  


class SocketFileTransmissionServer  
{  
public:  
    SocketFileTransmissionServer(void);  
    ~SocketFileTransmissionServer(void);  
    //int sockConn;  //socket
    int client_sockfd;
    int server_sockfd;
private:  
   //  struct recvBuf data1;  
  	// struct sentbuf data2;
   //  int needRecv;  
   //  int count;  
  
public:  
  
    int socketConnect(int PORT);  
  
	int transmit(char* filename);
 
    void socketDisconnect(void);  
};  
  
class SocketMatTransmissionServer  
{  
public:  
    SocketMatTransmissionServer(void);  
    ~SocketMatTransmissionServer(void);  
    //int sockConn;  //socket
    int client_sockfd;
    int server_sockfd;
private:  
    struct recvBuf data1;  
  	struct sentbuf data2;
    int needRecv;  
    int count;  
  
public:  
  
    int socketConnect(int PORT);  

    int receive(cv::Mat& image);  
  
	int transmit(cv::Mat image);

    void socketDisconnect(void);  
};  
  

SocketLogTransmissionServer::SocketLogTransmissionServer(void)  
{  
}  
  
  
SocketLogTransmissionServer::~SocketLogTransmissionServer(void)  
{  
}  

SocketFileTransmissionServer::SocketFileTransmissionServer(void)  
{  
}  
  
  
SocketFileTransmissionServer::~SocketFileTransmissionServer(void)  
{  
}  


SocketMatTransmissionServer::SocketMatTransmissionServer(void)  
{  
}  
  
  
SocketMatTransmissionServer::~SocketMatTransmissionServer(void)  
{  
}  
  

int SocketLogTransmissionServer::socketConnect(int PORT)  
{  
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_port = htons(PORT);  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    int on=1;  
    if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }

    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        perror("bind");  
        return -1;  
    }  

    printf("Socket Server Listening.\n");
  
    if(listen(server_sockfd,5) == -1)  
    {  
        perror("listen");  
        return -1;  
    }  

    struct sockaddr_in client_addr;  
    socklen_t length = sizeof(client_addr);  
  
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);  
    
    if(client_sockfd<0)  
    {  
        perror("connect");  
        return -1;  
    }  
    else  
    {  
        printf("connect successful!\n");  
        return 1;  
    }  
      
    close(server_sockfd);  
}  
  
  
void SocketLogTransmissionServer::socketDisconnect(void)  
{  
    close(client_sockfd);
    close(server_sockfd);

}  

int SocketFileTransmissionServer::socketConnect(int PORT)  
{  
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_port = htons(PORT);  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    int on=1;  
    if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }

    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        perror("bind");  
        return -1;  
    }  

    printf("Socket Server Listening.\n");
  
    if(listen(server_sockfd,5) == -1)  
    {  
        perror("listen");  
        return -1;  
    }  

    struct sockaddr_in client_addr;  
    socklen_t length = sizeof(client_addr);  
  
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);  
    if(client_sockfd<0)  
    {  
        perror("connect");  
        return -1;  
    }  
    else  
    {  
        printf("connect successful!\n");  
        return 1;  
    }  
      
    close(server_sockfd);  
}  
  
  
void SocketFileTransmissionServer::socketDisconnect(void)  
{  
    close(client_sockfd);
    close(server_sockfd);

}  
  
int SocketMatTransmissionServer::socketConnect(int PORT)  
{  
    server_sockfd = socket(AF_INET,SOCK_STREAM, 0);  
  
    struct sockaddr_in server_sockaddr;  
    server_sockaddr.sin_family = AF_INET;  
    server_sockaddr.sin_port = htons(PORT);  
    server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);  
  
    int on=1;  
    if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
    {  
        perror("setsockopt failed");  
        exit(EXIT_FAILURE);  
    }

    if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr))==-1)  
    {  
        perror("bind");  
        return -1;  
    }  

    printf("Socket Server Listening.\n");
  
    if(listen(server_sockfd,5) == -1)  
    {  
        perror("listen");  
        return -1;  
    }  

    struct sockaddr_in client_addr;  
    socklen_t length = sizeof(client_addr);  
  
    client_sockfd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);  
    if(client_sockfd<0)  
    {  
        perror("connect");  
        return -1;  
    }  
    else  
    {  
        printf("connect successful!\n");  
        return 1;  
    }  
      
    close(server_sockfd);  
}  
  
  
void SocketMatTransmissionServer::socketDisconnect(void)  
{  
    close(client_sockfd);
    close(server_sockfd);

}  
  
int SocketMatTransmissionServer::receive(cv::Mat& image)  
{  
    int returnflag = 0;  
    cv::Mat img(IMG_HEIGHT, IMG_WIDTH, CV_8UC3, cv::Scalar(0));  
    needRecv = sizeof(recvBuf);  
    count = 0;  
    memset(&data1,0,sizeof(data1));  
  
    for (int i = 0; i < PACKAGE_NUM; i++)  
    {  
        int pos = 0;  
        int len0 = 0;  
  
        while (pos < needRecv)  
        {   
            len0 = recv(client_sockfd, (char*)(&data1) + pos, needRecv - pos, 0);  
            if (len0 < 0)  
            {  
                printf("Server Recieve Data Failed!\n");  
                break;  
            }  
            pos += len0;  
        }  
  
        count = count + data1.flag;  
  
        int num1 = IMG_HEIGHT / PACKAGE_NUM * i;  
        for (int j = 0; j < IMG_HEIGHT / PACKAGE_NUM; j++)  
        {  
            int num2 = j * IMG_WIDTH * 3;  
            uchar* ucdata1 = img.ptr<uchar>(j + num1);  
            for (int k = 0; k < IMG_WIDTH * 3; k++)  
            {  
                ucdata1[k] = data1.buf[num2 + k];  
            }  
        }  
  
        if (data1.flag == 2)  
        {  
            if (count == PACKAGE_NUM + 1)  
            {  
                image = img;  
                returnflag = 1;  
                count = 0;  
            }  
            else  
            {  
                count = 0;  
                i = 0;  
            }  
        }  
    }  
    if(returnflag == 1)  
        return 1;  
    else  
        return -1;  
}

int SocketMatTransmissionServer::transmit(cv::Mat image)  
{  
	printf("Mat transmiting.\n");
    if (image.empty())  
    {  
        printf("empty image\n\n");  
        return -1;  
    }  

    if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC4)  
    {  
        printf("the image must satisfy : cols == IMG_WIDTH£¨%d£©  rows == IMG_HEIGHT£¨%d£© type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);  
        return -1;  
    }  

    for(int k = 0; k < PACKAGE_NUM; k++)   
    {  
        int num1 = IMG_HEIGHT / PACKAGE_NUM * k;  
        for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)  
        {  
            int num2 = i * IMG_WIDTH * 4;  
            uchar* ucdata2 = image.ptr<uchar>(i + num1);  
            for (int j = 0; j < IMG_WIDTH * 4; j++)  
            {  
                data2.buf[num2 + j] = ucdata2[j];  
            }  
        }  
  	
        if(k == PACKAGE_NUM - 1)  
            data2.flag = 2;  
        else  
            data2.flag = 1;  
  
        if (send(client_sockfd, (char *)(&data2), sizeof(data2), 0) < 0)  
        {  
            printf("send image error: %s(errno: %d)\n", strerror(errno), errno);  
            return -1;  
        }  
    }  
    printf("Mat transmit completed.\n");
}  

char* SocketLogTransmissionServer::receive(char* str)  
{  
	FD_ZERO(&fd);
    FD_SET(client_sockfd, &fd);
    maxsock = client_sockfd;
    select(maxsock+1, &fd, NULL, NULL, &tv);
    // return NULL;
    if (!FD_ISSET(client_sockfd, &fd))  return NULL;
    char buffer[STR_BUFFER_SIZE];
    bzero(buffer, STR_BUFFER_SIZE);
    if (recv(client_sockfd, buffer, STR_BUFFER_SIZE, 0) < 0)
    {
    	printf("Server receiving str failed.\n");
    	return NULL;
    }
    printf("String: %s Receiving successful!\n",  buffer);
    str[0] = '\0';
    strcat(str, buffer);
    return str;
}

int SocketLogTransmissionServer::transmit(char* str)  
{  
    char buffer[STR_BUFFER_SIZE];
    printf("String: %s transmiting.\n", str);
    bzero(buffer, STR_BUFFER_SIZE);
    strncpy(buffer, str, STR_BUFFER_SIZE);
    if (send(client_sockfd, buffer, STR_BUFFER_SIZE, 0) < 0)
    {
    	printf("Server sending str failed.\n");
    	return 1;
    }
    printf("String: %s Transmit successful!\n",  str);
    return 0;
}  

int SocketFileTransmissionServer::transmit(char* file_name)  
{  
    char buffer[FILE_BUFFER_SIZE];
    printf("File transmiting.\n");
    FILE *fp = NULL;
    do
    {
    	fp = fopen(file_name, "r");
    }while (!fp);
    bzero(buffer, FILE_BUFFER_SIZE);
    int length = 0;

    while((length = fread(buffer, sizeof(char), FILE_BUFFER_SIZE, fp)) > 0)
    {
    	if (send(client_sockfd, buffer, length, 0) < 0)
    	{
    		fclose(fp);
    		printf("File: %s sending failure.\n", file_name);
    		return 1;
    	}
    	bzero(buffer, FILE_BUFFER_SIZE);
    }

    fclose(fp);
    printf("File: %s sending successful.\n", file_name);
    return 0;
}

/**
 * This app demonstrates facial landmark tracking, estimation of the 3D pose
 * and fitting of the shape model of a 3D Morphable Model from a video stream,
 * and merging of the face texture.
 */
int main(int argc, char *argv[])
{
	fs::path modelfile, inputvideo, facedetector, landmarkdetector, mappingsfile, contourfile, edgetopologyfile, blendshapesfile;
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h",
				"display the help message")
			("morphablemodel,m", po::value<fs::path>(&modelfile)->required()->default_value("../share/sfm_shape_3448.bin"),
				"a Morphable Model stored as cereal BinaryArchive")
			("facedetector,f", po::value<fs::path>(&facedetector)->required()->default_value("../share/haarcascade_frontalface_alt2.xml"),
				"full path to OpenCV's face detector (haarcascade_frontalface_alt2.xml)")
			("landmarkdetector,l", po::value<fs::path>(&landmarkdetector)->required()->default_value("../share/face_landmarks_model_rcr_68.bin"),
				"learned landmark detection model")
			("mapping,p", po::value<fs::path>(&mappingsfile)->required()->default_value("../share/ibug2did.txt"),
				"landmark identifier to model vertex number mapping")
			("model-contour,c", po::value<fs::path>(&contourfile)->required()->default_value("../share/model_contours.json"),
				"file with model contour indices")
			("edge-topology,e", po::value<fs::path>(&edgetopologyfile)->required()->default_value("../share/sfm_3448_edge_topology.json"),
				"file with model's precomputed edge topology")
			("blendshapes,b", po::value<fs::path>(&blendshapesfile)->required()->default_value("../share/expression_blendshapes_3448.bin"),
				"file with blendshapes")
			("input,i", po::value<fs::path>(&inputvideo),
				"input video file. If not specified, camera 0 will be used.")
			;
		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
		if (vm.count("help")) {
			cout << "Usage: QuickFace [options]" << endl;
			cout << desc;
			return EXIT_FAILURE;
		}
		po::notify(vm);
	}
	catch (const po::error& e) {
		cout << "Error while parsing command-line arguments: " << e.what() << endl;
		cout << "Use --help to display a list of options." << endl;
		return EXIT_FAILURE;
	}

	// Load the Morphable Model and the LandmarkMapper:
	morphablemodel::MorphableModel morphable_model = morphablemodel::load_model(modelfile.string());
	core::LandmarkMapper landmark_mapper = mappingsfile.empty() ? core::LandmarkMapper() : core::LandmarkMapper(mappingsfile);

	fitting::ModelContour model_contour = contourfile.empty() ? fitting::ModelContour() : fitting::ModelContour::load(contourfile.string());
	fitting::ContourLandmarks ibug_contour = fitting::ContourLandmarks::load(mappingsfile.string());

	rcr::detection_model rcr_model;
	// Load the landmark detection model:
	try {
		rcr_model = rcr::load_detection_model(landmarkdetector.string());
	}
	catch (const cereal::Exception& e) {
		cout << "Error reading the RCR model " << landmarkdetector << ": " << e.what() << endl;
		return EXIT_FAILURE;
	}

	// Load the face detector from OpenCV:
	cv::CascadeClassifier face_cascade;
	if (!face_cascade.load(facedetector.string()))
	{
		cout << "Error loading the face detector " << facedetector << "." << endl;
		return EXIT_FAILURE;
	}
///////////////////////////////////////////////////////////////////////////////////////////
	printf("Initializing Socket Server:\n");

	SocketMatTransmissionServer socketMat;
	if (socketMat.socketConnect(12000) < 0)
	{
		return 0;
	}

	SocketLogTransmissionServer socketLog;
	if (socketLog.socketConnect(12001) < 0)
	{
		return 0;
	}

	SocketFileTransmissionServer socketFile;
	if (socketFile.socketConnect(12002) < 0)
	{
		return 0;
	}

	SocketFileTransmissionServer socketFile2;
	if (socketFile2.socketConnect(12003) < 0)
	{
		return 0;
	}
	//while(~socketMat.client_sockfd.isConnected());
	//cv::VideoCapture cap;
	//if (inputvideo.empty()) {
	//	cap.open(0); // no file given, open the default camera
	//}
	//else {
	//	cap.open(inputvideo.string());
	//}
///////////////////////////////////////////////////////////////////////////////////////////
	//if (!cap.isOpened()) {
		//cout << "Couldn't open the given file or camera 0." << endl;
		//return EXIT_FAILURE;
	//}

	printf("Socket Server Initialized.\n");

	vector<morphablemodel::Blendshape> blendshapes = morphablemodel::load_blendshapes(blendshapesfile.string());

	morphablemodel::EdgeTopology edge_topology = morphablemodel::load_edge_topology(edgetopologyfile.string());

	cv::namedWindow("video", 1);
	cv::namedWindow("render", 1);

	Mat frame, unmodified_frame;

	bool have_face = false;
	bool fileFlag = false;
	rcr::LandmarkCollection<Vec2f> current_landmarks;
	Rect current_facebox;
	WeightedIsomapAveraging isomap_averaging(60.f); // merge all triangles that are facing <60° towards the camera
	PcaCoefficientMerging pca_shape_merging;

	char* tempL = NULL;
	char key[STR_BUFFER_SIZE];
	bzero(key,STR_BUFFER_SIZE);

	// char * transTempChar = "Initialized!";
	// socketLog.transmit(transTempChar);

///////////////////////////////////////////////////////////////////////////////////////////////
	for (;;)
	{
		printf("Waiting transmition! \n");

		socketMat.receive(frame);
		if (frame.empty()) continue;

		// We do a quick check if the current face's width is <= 50 pixel. If it is, we re-initialise the tracking with the face detector.
		if (have_face && get_enclosing_bbox(rcr::to_row(current_landmarks)).width <= 50) {
			cout << "Reinitialising because the face bounding-box width is <= 50 px" << endl;
			have_face = false;
		}

		unmodified_frame = frame.clone();

		if (!have_face) {
			// Run the face detector and obtain the initial estimate using the mean landmarks:
			vector<Rect> detected_faces;
			face_cascade.detectMultiScale(unmodified_frame, detected_faces, 1.2, 2, 0, cv::Size(110, 110));
			if (detected_faces.empty()) {
				// cv::imshow("video", frame);
				// cv::waitKey(1);
				cv::Mat Em1(IMG_HEIGHT, IMG_WIDTH, CV_8UC4, cv::Scalar(0));
				socketMat.transmit(Em1);
				continue;
			}

			// transTempChar = "f";
			// socketLog.transmit(transTempChar);

			cv::rectangle(frame, detected_faces[0], { 255, 0, 0 });
			// Rescale the V&J facebox to make it more like an ibug-facebox:
			// (also make sure the bounding box is square, V&J's is square)
			Rect ibug_facebox = rescale_facebox(detected_faces[0], 0.85, 0.2);

			current_landmarks = rcr_model.detect(unmodified_frame, ibug_facebox);
			rcr::draw_landmarks(frame, current_landmarks, { 0, 0, 255 }); // red, initial landmarks

			have_face = true;

		}
		else {
			// We already have a face - track and initialise using the enclosing bounding
			// box from the landmarks from the last frame:
			auto enclosing_bbox = get_enclosing_bbox(rcr::to_row(current_landmarks));
			enclosing_bbox = make_bbox_square(enclosing_bbox);
			current_landmarks = rcr_model.detect(unmodified_frame, enclosing_bbox);
			rcr::draw_landmarks(frame, current_landmarks, { 255, 0, 0 }); // blue, the new optimised landmarks
		}

		printf("Prepare Rendering!\n"); 
		// Fit the 3DMM:
		fitting::RenderingParameters rendering_params;
		vector<float> shape_coefficients, blendshape_coefficients;
		vector<Vec2f> image_points;
		render::Mesh mesh;
		std::tie(mesh, rendering_params) = fitting::fit_shape_and_pose(morphable_model, blendshapes, rcr_to_eos_landmark_collection(current_landmarks), landmark_mapper, unmodified_frame.cols, unmodified_frame.rows, edge_topology, ibug_contour, model_contour, 3, 5, 15.0f, boost::none, shape_coefficients, blendshape_coefficients, image_points);

		// Extract the texture using the fitted mesh from this frame:
		Mat affine_cam = fitting::get_3x4_affine_camera_matrix(rendering_params, frame.cols, frame.rows);
		Mat isomap = render::extract_texture(mesh, affine_cam, unmodified_frame, true, render::TextureInterpolation::NearestNeighbour, 512);

		// Merge the isomaps - add the current one to the already merged ones:
		Mat merged_isomap = isomap_averaging.add_and_merge(isomap);
		// Same for the shape:
		shape_coefficients = pca_shape_merging.add_and_merge(shape_coefficients);
		auto merged_shape = morphable_model.get_shape_model().draw_sample(shape_coefficients) + morphablemodel::to_matrix(blendshapes) * Mat(blendshape_coefficients);
		render::Mesh merged_mesh = morphablemodel::sample_to_mesh(merged_shape, morphable_model.get_color_model().get_mean(), morphable_model.get_shape_model().get_triangle_list(), morphable_model.get_color_model().get_triangle_list(), morphable_model.get_texture_coordinates());
		
		// Render the model in a separate window using the estimated pose, shape and merged texture:
		Mat rendering;
		auto modelview_no_translation = rendering_params.get_modelview();
		modelview_no_translation[3][0] = 0;
		modelview_no_translation[3][1] = 0;
		std::tie(rendering, std::ignore) = render::render(merged_mesh, modelview_no_translation, glm::ortho(-130.0f, 130.0f, -130.0f, 130.0f), 640, 480, render::create_mipmapped_texture(merged_isomap), true, false, false);
		
		socketMat.transmit(rendering);
		printf("Transmit Rendered Image!\n"); 

		if (fileFlag)
		{
			socketFile.transmit("current_merged.obj");
			printf("Transmit file obj completed\n");
			socketFile2.transmit("current_merged.isomap.png");
			printf("Transmit file png completed\n");
			fileFlag = false;
			printf("ERROR: Server Failure!\n");
			break;
		}
		
		tempL = socketLog.receive(key);
		if (tempL == NULL) continue;
		if (tempL[0] == 'q') break;
		if (tempL[0] == 'r') {
			have_face = false;
			isomap_averaging = WeightedIsomapAveraging(60.f);
		}
		if (tempL[0] == 's') {
			printf("Receiving Key: s.\n");
			// save an obj + current merged isomap to the disk:
			render::Mesh neutral_expression = morphablemodel::sample_to_mesh(morphable_model.get_shape_model().draw_sample(shape_coefficients), morphable_model.get_color_model().get_mean(), morphable_model.get_shape_model().get_triangle_list(), morphable_model.get_color_model().get_triangle_list(), morphable_model.get_texture_coordinates());
			render::write_textured_obj(neutral_expression, "current_merged.obj");
			cv::imwrite("current_merged.isomap.png", merged_isomap);
			cv::waitKey(1);
			fileFlag = true;
		}
	}

	return EXIT_SUCCESS;
};
