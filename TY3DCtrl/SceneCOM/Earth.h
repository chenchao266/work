#pragma once

#include <string>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/ViewerBase>
#include <osgViewer/CompositeViewer>
#include <osgGA/StateSetManipulator>
#include <osgGA/TrackballManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgEarth/Map>
#include <osgEarth/MapNode>

#include <osg/Group>
#include <osg/Node>
#include <osgDB/ReadFile>
#include <osgUtil/optimizer>
#include <osgEarthUtil/Controls>
#include <osgEarthUtil/EarthManipulator>
#include "Compass.h"
using namespace osgEarth;
using namespace osgEarth::Util;
using namespace osgEarth::Util::Controls;

//#define  MINIMAP_ 
/************************************************************************/
/*        OSG               */
/************************************************************************/
class CEarth
{
public:
 
	~CEarth(void);
	// 构造函数获取上一级窗口句柄
	CEarth(HWND hWnd);
	// 初始化OSG，参数filename为需要加入的模型
	void InitOSG(std::string filename);
	// 初始化操作器
	void InitManipulators(void);
	// 初始化场景
	void InitSceneGraph(void);
	// 初始化相机配置
	void InitCameraConfig(void);
	// 建立窗口
	void SetupWindow(void);
	// 建立相机
	void SetupCamera(void);
	// 每帧绘制前整理
	void PreFrameUpdate(void);
	// 每帧绘制后处理
	void PostFrameUpdate(void);
    void ReplaceSceneData(osgEarth::MapNode* node);
    static void Render(void* ptr);

    osgViewer::Viewer* getViewer() { return mViewer; }
    Map* getMap() {return mMap.get();}
    Viewpoint getViewPoint(){return mEn->getViewpoint();}
    void setViewPoint(Viewpoint& vp){mEn->setViewpoint(vp, 0.5);}
private:
    bool mInit;
    std::string mProjectName;
    HWND m_hWnd;
    osgViewer::Viewer* mViewer;
    osg::ref_ptr<osg::Group> mRoot;
    osg::ref_ptr<MapNode> mMapNode;
    osg::ref_ptr<Map> mMap;
    osg::ref_ptr<EarthManipulator> mEn;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> mKeyswitchManipulator;
    //osg::ref_ptr<CCompass> mCompass;
private:
 
	void createControls();

    void createCompass();

public:
 

};
