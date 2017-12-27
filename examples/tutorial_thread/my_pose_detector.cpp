#include <chrono> // `std::chrono::` functions and classes, e.g. std::chrono::milliseconds
#include <string>
#include <thread> // std::this_thread
#include <vector>
// Other 3rdparty dependencies
// GFlags: DEFINE_bool, _int32, _int64, _uint64, _double, _string
#include <gflags/gflags.h>
#include <Spinnaker.h>
// Allow Google Flags in Ubuntu 14
#ifndef GFLAGS_GFLAGS_H_
    namespace gflags = google;
#endif

// OpenPose dependencies
#include <openpose/headers.hpp>

// See all the available parameter options withe the `--help` flag. E.g. `build/examples/openpose/openpose.bin --help`
// Note: This command will show you flags for other unnecessary 3rdparty files. Check only the flags for the OpenPose
// executable. E.g. for `openpose.bin`, look for `Flags from examples/openpose/openpose.cpp:`.
// Debugging/Other
DEFINE_int32(logging_level,             3,              "The logging level. Integer in the range [0, 255]. 0 will output any log() message, while"
                                                        " 255 will not output any. Current OpenPose library messages are in the range 0-4: 1 for"
                                                        " low priority messages and 4 for important ones.");
DEFINE_bool(disable_multi_thread,       false,          "It would slightly reduce the frame rate in order to highly reduce the lag. Mainly useful"
                                                        " for 1) Cases where it is needed a low latency (e.g. webcam in real-time scenarios with"
                                                        " low-range GPU devices); and 2) Debugging OpenPose when it is crashing to locate the"
                                                        " error.");
// OpenPose
DEFINE_string(model_folder,             "models/",      "Folder path (absolute or relative) where the models (pose, face, ...) are located.");
DEFINE_string(output_resolution,        "-1x-1",        "The image resolution (display and output). Use \"-1x-1\" to force the program to use the"
                                                        " input image resolution.");
DEFINE_int32(num_gpu,                   -1,             "The number of GPU devices to use. If negative, it will use all the available GPUs in your"
                                                        " machine.");
DEFINE_int32(num_gpu_start,             0,              "GPU device start number.");
DEFINE_int32(keypoint_scale,            0,              "Scaling of the (x,y) coordinates of the final pose data array, i.e. the scale of the (x,y)"
                                                        " coordinates that will be saved with the `write_keypoint` & `write_keypoint_json` flags."
                                                        " Select `0` to scale it to the original source resolution, `1`to scale it to the net output"
                                                        " size (set with `net_resolution`), `2` to scale it to the final output size (set with"
                                                        " `resolution`), `3` to scale it in the range [0,1], and 4 for range [-1,1]. Non related"
                                                        " with `scale_number` and `scale_gap`.");
// OpenPose Body Pose
DEFINE_bool(body_disable,               false,          "Disable body keypoint detection. Option only possible for faster (but less accurate) face"
                                                        " keypoint detection.");
DEFINE_string(model_pose,               "COCO",         "Model to be used. E.g. `COCO` (18 keypoints), `MPI` (15 keypoints, ~10% faster), "
                                                        "`MPI_4_layers` (15 keypoints, even faster but less accurate).");
DEFINE_string(net_resolution,           "-1x368",       "Multiples of 16. If it is increased, the accuracy potentially increases. If it is"
                                                        " decreased, the speed increases. For maximum speed-accuracy balance, it should keep the"
                                                        " closest aspect ratio possible to the images or videos to be processed. Using `-1` in"
                                                        " any of the dimensions, OP will choose the optimal aspect ratio depending on the user's"
                                                        " input value. E.g. the default `-1x368` is equivalent to `656x368` in 16:9 resolutions,"
                                                        " e.g. full HD (1980x1080) and HD (1280x720) resolutions.");
DEFINE_int32(scale_number,              1,              "Number of scales to average.");
DEFINE_double(scale_gap,                0.3,            "Scale gap between scales. No effect unless scale_number > 1. Initial scale is always 1."
                                                        " If you want to change the initial scale, you actually want to multiply the"
                                                        " `net_resolution` by your desired initial scale.");
// OpenPose Body Pose Heatmaps
DEFINE_bool(heatmaps_add_parts,         false,          "If true, it will add the body part heatmaps to the final op::Datum::poseHeatMaps array,"
                                                        " and analogously face & hand heatmaps to op::Datum::faceHeatMaps & op::Datum::handHeatMaps"
                                                        " (program speed will decrease). Not required for our library, enable it only if you intend"
                                                        " to process this information later. If more than one `add_heatmaps_X` flag is enabled, it"
                                                        " will place then in sequential memory order: body parts + bkg + PAFs. It will follow the"
                                                        " order on POSE_BODY_PART_MAPPING in `include/openpose/pose/poseParameters.hpp`.");
DEFINE_bool(heatmaps_add_bkg,           false,          "Same functionality as `add_heatmaps_parts`, but adding the heatmap corresponding to"
                                                        " background.");
DEFINE_bool(heatmaps_add_PAFs,          false,          "Same functionality as `add_heatmaps_parts`, but adding the PAFs.");
DEFINE_int32(heatmaps_scale,            2,              "Set 0 to scale op::Datum::poseHeatMaps in the range [-1,1], 1 for [0,1]; 2 for integer"
                                                        " rounded [0,255]; and 3 for no scaling.");
// OpenPose Face
DEFINE_bool(face,                       true,           "Enables face keypoint detection. It will share some parameters from the body pose, e.g."
                                                        " `model_folder`. Note that this will considerable slow down the performance and increse"
                                                        " the required GPU memory. In addition, the greater number of people on the image, the"
                                                        " slower OpenPose will be.");
DEFINE_string(face_net_resolution,      "368x368",      "Multiples of 16 and squared. Analogous to `net_resolution` but applied to the face keypoint"
                                                        " detector. 320x320 usually works fine while giving a substantial speed up when multiple"
                                                        " faces on the image.");
// OpenPose Hand
DEFINE_bool(hand,                       true,           "Enables hand keypoint detection. It will share some parameters from the body pose, e.g."
                                                        " `model_folder`. Analogously to `--face`, it will also slow down the performance, increase"
                                                        " the required GPU memory and its speed depends on the number of people.");
DEFINE_string(hand_net_resolution,      "368x368",      "Multiples of 16 and squared. Analogous to `net_resolution` but applied to the hand keypoint"
                                                        " detector.");
DEFINE_int32(hand_scale_number,         1,              "Analogous to `scale_number` but applied to the hand keypoint detector. Our best results"
                                                        " were found with `hand_scale_number` = 6 and `hand_scale_range` = 0.4");
DEFINE_double(hand_scale_range,         0.4,            "Analogous purpose than `scale_gap` but applied to the hand keypoint detector. Total range"
                                                        " between smallest and biggest scale. The scales will be centered in ratio 1. E.g. if"
                                                        " scaleRange = 0.4 and scalesNumber = 2, then there will be 2 scales, 0.8 and 1.2.");

DEFINE_bool(hand_tracking,              false,          "Adding hand tracking might improve hand keypoints detection for webcam (if the frame rate"
                                                        " is high enough, i.e. >7 FPS per GPU) and video. This is not person ID tracking, it"
                                                        " simply looks for hands in positions at which hands were located in previous frames, but"
                                                        " it does not guarantee the same person ID among frames");
// OpenPose Rendering
DEFINE_int32(part_to_show,              0,              "Prediction channel to visualize (default: 0). 0 for all the body parts, 1-18 for each body"
                                                        " part heat map, 19 for the background heat map, 20 for all the body part heat maps"
                                                        " together, 21 for all the PAFs, 22-40 for each body part pair PAF");
DEFINE_bool(disable_blending,           false,          "If enabled, it will render the results (keypoint skeletons or heatmaps) on a black"
                                                        " background, instead of being rendered into the original image. Related: `part_to_show`,"
                                                        " `alpha_pose`, and `alpha_pose`.");
// OpenPose Rendering Pose
DEFINE_double(render_threshold,         0.05,           "Only estimated keypoints whose score confidences are higher than this threshold will be"
                                                        " rendered. Generally, a high threshold (> 0.5) will only render very clear body parts;"
                                                        " while small thresholds (~0.1) will also output guessed and occluded keypoints, but also"
                                                        " more false positives (i.e. wrong detections).");
DEFINE_int32(render_pose,               1,              "Set to 0 for no rendering, 1 for CPU rendering (slightly faster), and 2 for GPU rendering"
                                                        " (slower but greater functionality, e.g. `alpha_X` flags). If rendering is enabled, it will"
                                                        " render both `outputData` and `cvOutputData` with the original image and desired body part"
                                                        " to be shown (i.e. keypoints, heat maps or PAFs).");
DEFINE_double(alpha_pose,               0.6,            "Blending factor (range 0-1) for the body part rendering. 1 will show it completely, 0 will"
                                                        " hide it. Only valid for GPU rendering.");
DEFINE_double(alpha_heatmap,            0.7,            "Blending factor (range 0-1) between heatmap and original frame. 1 will only show the"
                                                        " heatmap, 0 will only show the frame. Only valid for GPU rendering.");
// OpenPose Rendering Face
DEFINE_double(face_render_threshold,    0.4,            "Analogous to `render_threshold`, but applied to the face keypoints.");
DEFINE_int32(face_render,               -1,             "Analogous to `render_pose` but applied to the face. Extra option: -1 to use the same"
                                                        " configuration that `render_pose` is using.");
DEFINE_double(face_alpha_pose,          0.6,            "Analogous to `alpha_pose` but applied to face.");
DEFINE_double(face_alpha_heatmap,       0.7,            "Analogous to `alpha_heatmap` but applied to face.");
// OpenPose Rendering Hand
DEFINE_double(hand_render_threshold,    0.2,            "Analogous to `render_threshold`, but applied to the hand keypoints.");
DEFINE_int32(hand_render,               -1,             "Analogous to `render_pose` but applied to the hand. Extra option: -1 to use the same"
                                                        " configuration that `render_pose` is using.");
DEFINE_double(hand_alpha_pose,          0.6,            "Analogous to `alpha_pose` but applied to hand.");
DEFINE_double(hand_alpha_heatmap,       0.7,            "Analogous to `alpha_heatmap` but applied to hand.");
// Result Saving
DEFINE_string(write_images,             "",             "Directory to write rendered frames in `write_images_format` image format.");
DEFINE_string(write_images_format,      "png",          "File extension and format for `write_images`, e.g. png, jpg or bmp. Check the OpenCV"
                                                        " function cv::imwrite for all compatible extensions.");
DEFINE_string(write_video,              "",             "Full file path to write rendered frames in motion JPEG video format. It might fail if the"
                                                        " final path does not finish in `.avi`. It internally uses cv::VideoWriter.");
DEFINE_string(write_keypoint,           "",             "Directory to write the people body pose keypoint data. Set format with `write_keypoint_format`.");
DEFINE_string(write_keypoint_format,    "yml",          "File extension and format for `write_keypoint`: json, xml, yaml & yml. Json not available"
                                                        " for OpenCV < 3.0, use `write_keypoint_json` instead.");
DEFINE_string(write_keypoint_json,      "",             "Directory to write people pose data in *.json format, compatible with any OpenCV version.");
DEFINE_string(write_coco_json,          "",             "Full file path to write people pose data with *.json COCO validation format.");
DEFINE_string(write_heatmaps,           "",             "Directory to write body pose heatmaps in *.png format. At least 1 `add_heatmaps_X` flag"
                                                        " must be enabled.");
DEFINE_string(write_heatmaps_format,    "png",          "File extension and format for `write_heatmaps`, analogous to `write_images_format`."
                                                        " For lossless compression, recommended `png` for integer `heatmaps_scale` and `float` for"
                                                        " floating values.");                                                     
                          


struct UserDatum : public op::Datum
{
    bool boolThatUserNeedsForSomeReason;

    UserDatum(const bool boolThatUserNeedsForSomeReason_ = false) :
        boolThatUserNeedsForSomeReason{boolThatUserNeedsForSomeReason_}
    {}
};


class WUserInput : public op::WorkerProducer<std::shared_ptr<std::vector<UserDatum>>>
{

public:
    WUserInput() :
        initialized{false}
    {  
    }

    ~WUserInput() 
    {
        try
        {
            if (initialized)
            {
                // End acquisition for each camera
                // Notice that what is usually a one-step process is now two steps
                // because of the additional step of selecting the camera. It is worth
                // repeating that camera selection needs to be done once per loop.
                // It is possible to interact with cameras through the camera list with
                // GetByIndex(); this is an alternative to retrieving cameras as
                // Spinnaker::CameraPtr objects that can be quick and easy for small tasks.
                //
                for (auto i = 0; i < mCameraList.GetSize(); i++)
                    mCameraList.GetByIndex(i)->EndAcquisition();

                for (auto i = 0; i < mCameraList.GetSize(); i++)
                {
                    // Select camera
                    auto cameraPtr = mCameraList.GetByIndex(i);

                    // Retrieve GenICam nodemap
                    auto& iNodeMap = cameraPtr->GetNodeMap();

                    // // Disable chunk data
                    // result = disableChunkData(iNodeMap);
                    // // if (result < 0)
                    // //     return result;

                    // // Reset trigger
                    // auto result = resetTrigger(iNodeMap);
                    // if (result < 0)
                    //     op::error("Error happened..." + std::to_string(result), __LINE__, __FUNCTION__, __FILE__);

                    // Deinitialize each camera
                    // Each camera must be deinitialized separately by first
                    // selecting the camera and then deinitializing it.
                    cameraPtr->DeInit();
                }

                op::log("Completed. Releasing...", op::Priority::High);

                // Clear camera list before releasing mSystemPtr
                mCameraList.Clear();

                // Release mSystemPtr
                mSystemPtr->ReleaseInstance();
            }

            op::log("Done! Exitting...", op::Priority::High);
        }
        catch (const Spinnaker::Exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
        catch (const std::exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }

    }

    void initializationOnThread() {

        try
        {
            initialized = true;

            // Print application build information
            op::log(std::string{ "Application build date: " } + __DATE__ + " " + __TIME__, op::Priority::High);

            // Retrieve singleton reference to mSystemPtr object
            mSystemPtr = Spinnaker::System::GetInstance();

            // Retrieve list of cameras from the mSystemPtr
            mCameraList = mSystemPtr->GetCameras();

            unsigned int numCameras = mCameraList.GetSize();

            op::log("Number of cameras detected: " + std::to_string(numCameras), op::Priority::High);

            // Finish if there are no cameras
            if (numCameras == 0)
            {
                // Clear camera list before releasing mSystemPtr
                mCameraList.Clear();

                // Release mSystemPtr
                mSystemPtr->ReleaseInstance();

                op::log("Not enough cameras!\nPress Enter to exit...", op::Priority::High);
                getchar();

                op::error("No cameras detected.", __LINE__, __FUNCTION__, __FILE__);
            }
            op::log("Camera system initialized...", op::Priority::High);

            //
            // Retrieve transport layer nodemaps and print device information for
            // each camera
            //
            // *** NOTES ***
            // This example retrieves information from the transport layer nodemap
            // twice: once to print device information and once to grab the device
            // serial number. Rather than caching the nodemap, each nodemap is
            // retrieved both times as needed.
            //
            op::log("\n*** DEVICE INFORMATION ***\n", op::Priority::High);

            for (int i = 0; i < mCameraList.GetSize(); i++)
            {
                // Select camera
                auto cameraPtr = mCameraList.GetByIndex(i);

                // Retrieve TL device nodemap
                auto& iNodeMapTLDevice = cameraPtr->GetTLDeviceNodeMap();

                // Print device information
                // auto result = printDeviceInfo(iNodeMapTLDevice, i);
                // if (result < 0)
                //     op::error("Result > 0, error " + std::to_string(result) + " occurred...", __LINE__, __FUNCTION__, __FILE__);
            }

            for (auto i = 0; i < mCameraList.GetSize(); i++)
            {
                // Select camera
                auto cameraPtr = mCameraList.GetByIndex(i);

                // Initialize each camera
                // You may notice that the steps in this function have more loops with
                // less steps per loop; this contrasts the acquireImages() function
                // which has less loops but more steps per loop. This is done for
                // demonstrative purposes as both work equally well.
                // Later: Each camera needs to be deinitialized once all images have been
                // acquired.
                cameraPtr->Init();

                // Remove buffer --> Always get newest frame
                Spinnaker::GenApi::INodeMap& snodeMap = cameraPtr->GetTLStreamNodeMap();
                Spinnaker::GenApi::CEnumerationPtr ptrBufferHandlingMode = snodeMap.GetNode("StreamBufferHandlingMode");
                if (!Spinnaker::GenApi::IsAvailable(ptrBufferHandlingMode) || !Spinnaker::GenApi::IsWritable(ptrBufferHandlingMode))
                    op::error("Unable to change buffer handling mode", __LINE__, __FUNCTION__, __FILE__);

                Spinnaker::GenApi::CEnumEntryPtr ptrBufferHandlingModeNewest = ptrBufferHandlingMode->GetEntryByName("NewestFirstOverwrite");
                if (!Spinnaker::GenApi::IsAvailable(ptrBufferHandlingModeNewest) || !IsReadable(ptrBufferHandlingModeNewest))
                    op::error("Unable to set buffer handling mode to newest (entry 'NewestFirstOverwrite' retrieval). Aborting...", __LINE__, __FUNCTION__, __FILE__);
                int64_t bufferHandlingModeNewest = ptrBufferHandlingModeNewest->GetValue();

                ptrBufferHandlingMode->SetIntValue(bufferHandlingModeNewest);
            }

            // Prepare each camera to acquire images
            //
            // *** NOTES ***
            // For pseudo-simultaneous streaming, each camera is prepared as if it
            // were just one, but in a loop. Notice that cameras are selected with
            // an index. We demonstrate pseduo-simultaneous streaming because true
            // simultaneous streaming would require multiple process or threads,
            // which is too complex for an example.
            //
            // Serial numbers are the only persistent objects we gather in this
            // example, which is why a std::vector is created.
            std::vector<Spinnaker::GenICam::gcstring> strSerialNumbers(mCameraList.GetSize());
            for (auto i = 0u; i < strSerialNumbers.size(); i++)
            {
                // Select camera
                auto cameraPtr = mCameraList.GetByIndex(i);

                // Set acquisition mode to continuous
                Spinnaker::GenApi::CEnumerationPtr ptrAcquisitionMode = cameraPtr->GetNodeMap().GetNode("AcquisitionMode");
                if (!Spinnaker::GenApi::IsAvailable(ptrAcquisitionMode) || !Spinnaker::GenApi::IsWritable(ptrAcquisitionMode))
                    op::error("Unable to set acquisition mode to continuous (node retrieval; camera " + std::to_string(i) + "). Aborting...", __LINE__, __FUNCTION__, __FILE__);

                Spinnaker::GenApi::CEnumEntryPtr ptrAcquisitionModeContinuous = ptrAcquisitionMode->GetEntryByName("Continuous");
                if (!Spinnaker::GenApi::IsAvailable(ptrAcquisitionModeContinuous) || !Spinnaker::GenApi::IsReadable(ptrAcquisitionModeContinuous))
                    op::error("Unable to set acquisition mode to continuous (entry 'continuous' retrieval " + std::to_string(i) + "). Aborting...", __LINE__, __FUNCTION__, __FILE__);

                int64_t acquisitionModeContinuous = ptrAcquisitionModeContinuous->GetValue();

                ptrAcquisitionMode->SetIntValue(acquisitionModeContinuous);

                op::log("Camera " + std::to_string(i) + " acquisition mode set to continuous...", op::Priority::High);

                // Begin acquiring images
                cameraPtr->BeginAcquisition();

                op::log("Camera " + std::to_string(i) + " started acquiring images...", op::Priority::High);

                // Retrieve device serial number for filename
                strSerialNumbers[i] = "";

                Spinnaker::GenApi::CStringPtr ptrStringSerial = cameraPtr->GetTLDeviceNodeMap().GetNode("DeviceSerialNumber");

                if (Spinnaker::GenApi::IsAvailable(ptrStringSerial) && Spinnaker::GenApi::IsReadable(ptrStringSerial))
                {
                    strSerialNumbers[i] = ptrStringSerial->GetValue();
                    op::log("Camera " + std::to_string(i) + " serial number set to " + strSerialNumbers[i].c_str() + "...", op::Priority::High);
                }
                op::log(" ", op::Priority::High);
            }

            op::log("\nRunning for all cameras...\n\n*** IMAGE ACQUISITION ***\n", op::Priority::High);
        }
        catch (const Spinnaker::Exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
        catch (const std::exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }

    }

    /*
     * This function converts between Spinnaker::ImagePtr container to cv::Mat container used in OpenCV.
    */
    cv::Mat pointGreyToCvMat(const Spinnaker::ImagePtr &imagePtr)
    {
        try
        {
            const auto XPadding = imagePtr->GetXPadding();
            const auto YPadding = imagePtr->GetYPadding();
            const auto rowsize = imagePtr->GetWidth();
            const auto colsize = imagePtr->GetHeight();

            // image data contains padding. When allocating cv::Mat container size, you need to account for the X,Y image data padding.
            return cv::Mat((int)(colsize + YPadding), (int)(rowsize + XPadding), CV_8UC3, imagePtr->GetData(), imagePtr->GetStride());
        }
        catch (const std::exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return cv::Mat();
        }
    }

    // This function acquires and displays images from each device.
    std::vector<cv::Mat> acquireImages(Spinnaker::CameraList &cameraList)
    {
        try
        {
           
            std::vector<cv::Mat> cvMats;
            // Retrieve, convert, and return an image for each camera
            // In order to work with simultaneous camera streams, nested loops are
            // needed. It is important that the inner loop be the one iterating
            // through the cameras; otherwise, all images will be grabbed from a
            // single camera before grabbing any images from another.

            // Get cameras
            std::vector<Spinnaker::CameraPtr> cameraPtrs(cameraList.GetSize());
            for (auto i = 0u; i < cameraPtrs.size(); i++)
                cameraPtrs.at(i) = cameraList.GetByIndex(i);
            
            std::vector<Spinnaker::ImagePtr> imagePtrs(cameraPtrs.size());

            
            auto sleepSeconds = 100;
            op::log("Sleep for "+ std::to_string(sleepSeconds) + "ms until get next image",
                            op::Priority::High, __LINE__, __FUNCTION__, __FILE__);
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepSeconds));


            // Getting frames
            // Retrieve next received image and ensure image completion
            // Spinnaker::ImagePtr imagePtr = cameraPtrs.at(i)->GetNextImage();
            // Clean buffer + retrieve next received image + ensure image completion
            auto durationMs = 0.;
            // for (auto counter = 0 ; counter < 10 ; counter++)
            while (durationMs < 1.)
            {
                const auto begin = std::chrono::high_resolution_clock::now();
                for (auto i = 0u; i < cameraPtrs.size(); i++)
                    imagePtrs.at(i) = cameraPtrs.at(i)->GetNextImage();
                durationMs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-begin).count() * 1e-6;
                // op::log("Time extraction (ms): " + std::to_string(durationMs), op::Priority::High);
            }

            // Original format -> RGB8
            bool imagesExtracted = true;
            for (auto& imagePtr : imagePtrs)
            {
                if (imagePtr->IsIncomplete())
                {
                    op::log("Image incomplete with image status " + std::to_string(imagePtr->GetImageStatus()) + "...",
                            op::Priority::High, __LINE__, __FUNCTION__, __FILE__);
                    imagesExtracted = false;
                    break;
                }
                else
                {   

                    op::log("Image width " + std::to_string(imagePtr->GetWidth()) + " Image Height " + std::to_string(imagePtr->GetHeight()) 
                        + " Image XPadding " + std::to_string(imagePtr->GetXPadding()) + " Image YPadding " + std::to_string(imagePtr->GetYPadding()),
                            op::Priority::High, __LINE__, __FUNCTION__, __FILE__);

                    // Print image information
                    // Convert image to RGB
                    // Interpolation methods
                    // http://softwareservices.ptgrey.com/Spinnaker/latest/group___spinnaker_defs.html
                    // DEFAULT     Default method.
                    // NO_COLOR_PROCESSING     No color processing.
                    // NEAREST_NEIGHBOR    Fastest but lowest quality. Equivalent to FLYCAPTURE_NEAREST_NEIGHBOR_FAST in FlyCapture.
                    // EDGE_SENSING    Weights surrounding pixels based on localized edge orientation.
                    // HQ_LINEAR   Well-balanced speed and quality.
                    // RIGOROUS    Slowest but produces good results.
                    // IPP     Multi-threaded with similar results to edge sensing.
                    // DIRECTIONAL_FILTER  Best quality but much faster than rigorous.
                    // Colors
                    // http://softwareservices.ptgrey.com/Spinnaker/latest/group___camera_defs__h.html#ggabd5af55aaa20bcb0644c46241c2cbad1a33a1c8a1f6dbcb4a4eaaaf6d4d7ff1d1
                    // PixelFormat_BGR8
                    // Time tests
                    // const auto reps = 1e3;
                    // // const auto reps = 1e2; // for RIGOROUS & DIRECTIONAL_FILTER
                    // const auto begin = std::chrono::high_resolution_clock::now();
                    // for (auto asdf = 0 ; asdf < reps ; asdf++){
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::DEFAULT); // ~ 1.5 ms but pixeled
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::NO_COLOR_PROCESSING); // ~0.5 ms but BW
                    imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::HQ_LINEAR); // ~6 ms, looks as good as best
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::EDGE_SENSING); // ~2 ms default << edge << best
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::RIGOROUS); // ~115, too slow
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::IPP); // ~2 ms, slightly worse than HQ_LINEAR
                    // imagePtr = imagePtr->Convert(Spinnaker::PixelFormat_BGR8, Spinnaker::DIRECTIONAL_FILTER); // ~30 ms, ideally best quality?
                    // imagePtr = imagePtr;
                    // }
                    // durationMs = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()-begin).count() * 1e-6;
                    // op::log("Time conversion (ms): " + std::to_string(durationMs / reps), op::Priority::High);
                }
            }

            // Convert to cv::Mat
            if (imagesExtracted)
            {
                for (auto i = 0u; i < imagePtrs.size(); i++)
                {
                    // Baseline
                    // cvMats.emplace_back(pointGreyToCvMat(imagePtrs.at(i)).clone());
                    // Undistort
                    // http://docs.opencv.org/2.4/modules/imgproc/doc/geometric_transformations.html#undistort
                    cvMats.emplace_back(pointGreyToCvMat(imagePtrs.at(i)).clone());
                }
            }

            return cvMats;
        }
        catch (Spinnaker::Exception &e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return {};
        }
        catch (const std::exception& e)
        {
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return {};
        }
    }


    std::shared_ptr<std::vector<UserDatum>> workProducer()
    {
        try
        {
            // Profiling speed
            const auto profilerKey = op::Profiler::timerInit(__LINE__, __FUNCTION__, __FILE__);
            // Get image from each camera
            const auto cvMats = acquireImages(mCameraList);
            // Images to userDatum
            auto datatums = std::make_shared<std::vector<UserDatum>>(cvMats.size());
            for (auto i = 0u ; i < cvMats.size() ; i++) {
                op::log("Image width " + std::to_string(cvMats.at(i).cols) + " Image Height " + std::to_string(cvMats.at(i).rows));
                datatums->at(i).cvInputData = cvMats.at(i);
            }
            // Profiling speed
            if (!cvMats.empty()) 
            {
                op::Profiler::timerEnd(profilerKey);
                op::Profiler::printAveragedTimeMsOnIterationX(profilerKey, __LINE__, __FUNCTION__, __FILE__, 100);
            }
            op::log("Producing Pictures ...  with dataums.size()  " + std::to_string(datatums->size()));
            // Return Datum
            return datatums;
        }
        catch (const Spinnaker::Exception& e)
        {
            this->stop();
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return nullptr;
        }
        catch (const std::exception& e)
        {
            this->stop();
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
            return nullptr;
        }
    }

private:
    bool initialized;
    Spinnaker::CameraList mCameraList;
    Spinnaker::SystemPtr mSystemPtr;
};


// This worker will just invert the image
class WUserPostProcessing : public op::Worker<std::shared_ptr<std::vector<UserDatum>>>
{
public:
    WUserPostProcessing()
    {
    }

    void initializationOnThread() {

    }

    void work(std::shared_ptr<std::vector<UserDatum>>& datumsPtr)
    {
        // User's post-processing (after OpenPose processing & before OpenPose outputs) here
            // datum.cvOutputData: rendered frame with pose or heatmaps
            // datum.poseKeypoints: Array<float> with the estimated pose
        try
        {
            // do nothing       
          }
        catch (const std::exception& e)
        {
            op::log("Some kind of unexpected error happened.");
            this->stop();
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
private:

};

// This worker will just read and return all the jpg files in a directory
class WUserOutput : public op::WorkerConsumer<std::shared_ptr<std::vector<UserDatum>>>
{
public:
    void initializationOnThread() {
    }

    void workConsumer(const std::shared_ptr<std::vector<UserDatum>>& datumsPtr)
    {
        try
        {
            // User's displaying/saving/other processing here
                // datum.cvOutputData: rendered frame with pose or heatmaps
                // datum.poseKeypoints: Array<float> with the estimated pose
            if (datumsPtr != nullptr && !datumsPtr->empty())
            {   

                op::log("Deal with data ");
                cv::imshow("User worker GUI", datumsPtr->at(0).cvOutputData);
                // It displays the image and sleeps at least 1 ms (it usually sleeps ~5-10 msec to display the image)
                cv::waitKey(1);
                op::log("Array floats of the datum is ");
            }
        }
        catch (const std::exception& e)
        {
            op::log("Some kind of unexpected error happened.");
            this->stop();
            op::error(e.what(), __LINE__, __FUNCTION__, __FILE__);
        }
    }
};

int openPoseTutorialThread4()
{
    // logging_level
    op::check(0 <= FLAGS_logging_level && FLAGS_logging_level <= 255, "Wrong logging_level value.",
              __LINE__, __FUNCTION__, __FILE__);
    op::ConfigureLog::setPriorityThreshold((op::Priority)FLAGS_logging_level);
    // op::ConfigureLog::setPriorityThreshold(op::Priority::None); // To print all logging messages

    op::log("Starting pose estimation demo.", op::Priority::High);
    const auto timerBegin = std::chrono::high_resolution_clock::now();

    // Applying user defined configuration - Google flags to program variables
    // outputSize
    const auto outputSize = op::flagsToPoint(FLAGS_output_resolution, "-1x-1");
    // netInputSize
    const auto netInputSize = op::flagsToPoint(FLAGS_net_resolution, "-1x368");
    // faceNetInputSize
    const auto faceNetInputSize = op::flagsToPoint(FLAGS_face_net_resolution, "368x368 (multiples of 16)");
    // handNetInputSize
    const auto handNetInputSize = op::flagsToPoint(FLAGS_hand_net_resolution, "368x368 (multiples of 16)");
    // poseModel
    const auto poseModel = op::flagsToPoseModel(FLAGS_model_pose);
    // keypointScale
    const auto keypointScale = op::flagsToScaleMode(FLAGS_keypoint_scale);
    // heatmaps to add
    const auto heatMapTypes = op::flagsToHeatMaps(FLAGS_heatmaps_add_parts, FLAGS_heatmaps_add_bkg,
                                                  FLAGS_heatmaps_add_PAFs);
    const auto heatMapScale = op::flagsToHeatMapScaleMode(FLAGS_heatmaps_scale);
    // Enabling Google Logging
    const bool enableGoogleLogging = true;
    // Logging
    op::log("", op::Priority::Low, __LINE__, __FUNCTION__, __FILE__);

    // Initializing the user custom classes
    // Frames producer (e.g. video, webcam, ...)
    auto wUserInput = std::make_shared<WUserInput>();
    // Processing
    auto wUserPostProcessing = std::make_shared<WUserPostProcessing>();
    // GUI (Display)
    auto wUserOutput = std::make_shared<WUserOutput>();

    op::Wrapper<std::vector<UserDatum>> opWrapper;
    // Add custom input
    const auto workerInputOnNewThread = true;
    opWrapper.setWorkerInput(wUserInput, workerInputOnNewThread);
    // Add custom processing
    const auto workerProcessingOnNewThread = true;
    opWrapper.setWorkerPostProcessing(wUserPostProcessing, workerProcessingOnNewThread);
    // Add custom output
    const auto workerOutputOnNewThread = true;
    opWrapper.setWorkerOutput(wUserOutput, workerOutputOnNewThread);
    // Configure OpenPose
    const op::WrapperStructPose wrapperStructPose{!FLAGS_body_disable, netInputSize, outputSize, keypointScale,
                                                  FLAGS_num_gpu, FLAGS_num_gpu_start, FLAGS_scale_number,
                                                  (float)FLAGS_scale_gap, op::flagsToRenderMode(FLAGS_render_pose),
                                                  poseModel, !FLAGS_disable_blending, (float)FLAGS_alpha_pose,
                                                  (float)FLAGS_alpha_heatmap, FLAGS_part_to_show, FLAGS_model_folder,
                                                  heatMapTypes, heatMapScale, (float)FLAGS_render_threshold,
                                                  enableGoogleLogging};

    // Consumer (comment or use default argument to disable any output)
    const bool displayGui = false;
    const bool guiVerbose = true;
    const bool fullScreen = false;
    const op::WrapperStructOutput wrapperStructOutput{displayGui, guiVerbose, fullScreen, FLAGS_write_keypoint,
                                                      op::stringToDataFormat(FLAGS_write_keypoint_format), FLAGS_write_keypoint_json,
                                                      FLAGS_write_coco_json, FLAGS_write_images, FLAGS_write_images_format, FLAGS_write_video,
                                                      FLAGS_write_heatmaps, FLAGS_write_heatmaps_format};
    // Configure wrapper
    opWrapper.configure(wrapperStructPose, op::WrapperStructFace{}, op::WrapperStructHand{}, op::WrapperStructInput{},
                        wrapperStructOutput);
    // Set to single-thread running (to debug and/or reduce latency)
    if (FLAGS_disable_multi_thread)
       opWrapper.disableMultiThreading();

    op::log("Starting thread(s)", op::Priority::Max);
    // Start, run & stop threads
    opWrapper.exec();  // It blocks this thread until all threads have finished

    // Measuring total time
    const auto now = std::chrono::high_resolution_clock::now();
    const auto totalTimeSec = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(now-timerBegin).count()
                            * 1e-9;
    const auto message = "Real-time pose estimation demo successfully finished. Total time: "
                       + std::to_string(totalTimeSec) + " seconds.";
    op::log(message, op::Priority::High);

    return 0;
}

int main(int argc, char *argv[])
{
    // Parsing command line flags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Running openPoseTutorialThread4
    return openPoseTutorialThread4();
}
