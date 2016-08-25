#include "stdafx.h"
#include "InputManager.h"
#include "BaseApplication.h"
#include "EditPointor.h"
#include "MainFrm.h"

InputManager *InputManager::PmInstance=NULL;


InputManager::InputManager(EditScene *scene)
{
	h=0;w=0;c=0;l=0;t=0;
	mscene=scene;
	mEditPointor=new EditPointor(mscene);
	BaseApplication::getInstance()->mRoot->addFrameListener(mEditPointor);
	viewSpeed=10.0;
	mouseZViewSpeed=5.0;
	viewMode=1;    
	int pressedMB=-1;
	PressedKey=0x0000;
	oldMousePos=Ogre::Vector3(-1,-1,-1);
	newMousePos=Ogre::Vector3(-1,-1,-1);
	PmInstance=this;
	mHero=Hero::getInstance();
	
}

InputManager::~InputManager()
{

}

InputManager* InputManager::getInstance()
{
	return PmInstance;	
}


bool InputManager::frameStarted( const Ogre::FrameEvent &evt )
{
	upDateSoundSystem(evt.timeSinceLastFrame);
	if (viewMode==1)          //编辑模式//////
	{
		if (PressedKey&0x0001)    //w按下
		{
			mscene->cameraNode->translate(viewSpeed*mscene->mCamera->getDirection(), Ogre::Node::TS_LOCAL);
		}
		if (PressedKey&0x0002)    //s按下
		{
			mscene->cameraNode->translate(-viewSpeed*mscene->mCamera->getDirection(), Ogre::Node::TS_LOCAL);
		}
		if (PressedKey&0x0004)    //a按下
		{
			mscene->cameraNode->translate(-viewSpeed*mscene->mCamera->getRealRight(), Ogre::Node::TS_WORLD);
		}
		if (PressedKey&0x0008)    //d按下
		{
			mscene->cameraNode->translate(viewSpeed*mscene->mCamera->getRealRight(), Ogre::Node::TS_WORLD);
		}

		if (pressedMB==1 && mscene->bTerrainCreated)   //left mouse button down
		{
			
			if (mscene->mTerrainEditMode==TerrainUp||mscene->mTerrainEditMode==TerrainDown)
			{
				unsigned int h,w,c;    //窗口信息//
				int l,t;
				BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
				CPoint point;
				GetCursorPos(&point);
				Ogre::Ray mouseRay =mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));
				TerrainGroup::RayResult rayResult = mscene->mTerrainGroup->rayIntersects(mouseRay);
				if (rayResult.hit)
				{
					TerrainGroup::TerrainList terrainList;
					Real brushSizeWorldSpace = mscene->terrainSize * mscene->BrushSize;
					Sphere sphere(rayResult.position, brushSizeWorldSpace);
					mscene->mTerrainGroup->sphereIntersects(sphere, &terrainList);

					Ogre::Vector3 tsPos;
					
				    float mBrushSizeTerrainSpace = mscene->BrushSize;
					for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
					ti != terrainList.end(); ++ti)
					{
						//float brushIntensity = evt.timeSinceLastFrame * (mscene->BrushStrength )* (mscene->mTerrainEditMode==TerrainUp? 1 : -1);
						// translate our cursor position to vertex indexes
						Real terrainSize = ((*ti)->getSize() - 1);					
						(*ti)->getTerrainPosition(rayResult.position, &tsPos);
						long startx = (tsPos.x - mBrushSizeTerrainSpace) * terrainSize;
						long starty = (tsPos.y - mBrushSizeTerrainSpace) * terrainSize;
						long endx = (tsPos.x + mBrushSizeTerrainSpace) * terrainSize;
						long endy= (tsPos.y + mBrushSizeTerrainSpace) * terrainSize;
						startx = std::max(startx, 0L);
						starty = std::max(starty, 0L);
						endx = std::min(endx, (long)terrainSize);
						endy = std::min(endy, (long)terrainSize);
						for (long y = starty; y <= endy; ++y)
						{
							for (long x = startx; x <= endx; ++x)
							{
								Real tsXdist = (x / terrainSize) - tsPos.x;
								Real tsYdist = (y / terrainSize)  - tsPos.y;

								Real weight = std::min((Real)1.0, 
									Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSizeTerrainSpace));
								weight = 1.0 - (weight * weight);

								float addedHeight = weight * evt.timeSinceLastFrame * mscene->BrushStrength;
								float newheight;
								if (mscene->mTerrainEditMode==TerrainUp)
									newheight = (*ti)->getHeightAtPoint(x, y) + addedHeight;
								else
									newheight =  (*ti)->getHeightAtPoint(x, y) - addedHeight;
								 (*ti)->setHeightAtPoint(x, y, newheight);

							 }
						 }
					  }

					} //rayResult.hit
		
				} //地形上升或下降/////

				if ((mscene->mTerrainEditMode==TerrainPlatUp || mscene->mTerrainEditMode == TerrainPlatDown) && mscene->mSplatImage>=1)
				{
					unsigned int h,w,c;    //窗口信息//
					int l,t;
					BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
					CPoint point;
					GetCursorPos(&point);
					Ogre::Ray mouseRay =mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));
					TerrainGroup::RayResult rayResult = mscene->mTerrainGroup->rayIntersects(mouseRay);
					if (rayResult.hit)
					{
						TerrainGroup::TerrainList terrainList;
						Real brushSizeWorldSpace = mscene->terrainSize * mscene->BrushSize;
						Sphere sphere(rayResult.position, brushSizeWorldSpace);
						mscene->mTerrainGroup->sphereIntersects(sphere, &terrainList);

						Ogre::Vector3 tsPos;

						float mBrushSizeTerrainSpace = mscene->BrushSize;
						for (TerrainGroup::TerrainList::iterator ti = terrainList.begin();
							ti != terrainList.end(); ++ti)
						{
							(*ti)->getTerrainPosition(rayResult.position, &tsPos);
							//float brushIntensity = evt.timeSinceLastFrame * mscene->BrushStrength*10;
							TerrainLayerBlendMap* layer = (*ti)->getLayerBlendMap(mscene->mSplatImage); //编辑第二层///					
							Real imgSize = (*ti)->getLayerBlendMapSize();
							long startx = (tsPos.x - mBrushSizeTerrainSpace) * imgSize;
							long starty = (tsPos.y - mBrushSizeTerrainSpace) * imgSize;
							long endx = (tsPos.x + mBrushSizeTerrainSpace) * imgSize;
							long endy= (tsPos.y + mBrushSizeTerrainSpace) * imgSize;
							startx = std::max(startx, 0L);
							starty = std::max(starty, 0L);
							endx = std::min(endx, (long)imgSize);
							endy = std::min(endy, (long)imgSize);
							for (long y = starty; y <= endy; ++y)
							{
								for (long x = startx; x <= endx; ++x)
								{
									Real tsXdist = (x / imgSize) - tsPos.x;
									Real tsYdist = (y / imgSize)  - tsPos.y;

									Real weight = std::min((Real)1.0, 
										Math::Sqrt(tsYdist * tsYdist + tsXdist * tsXdist) / Real(0.5 * mBrushSizeTerrainSpace));
									weight = 1.0 - (weight * weight);

									float paint = weight * evt.timeSinceLastFrame * mscene->BrushStrength * 0.005;
									size_t imgY = imgSize - y;
									float val;
									if (mscene->mTerrainEditMode ==  TerrainPlatUp)
									{
										val = layer->getBlendValue(x, imgY) + paint;
									}
									if (mscene->mTerrainEditMode == TerrainPlatDown)
									{
										val = layer->getBlendValue(x, imgY) - paint;
									}											
									val = Math::Clamp(val, 0.0f, 1.0f);
									layer->setBlendValue(x, imgY, val);
								}
							}
							layer->update();
							
						} //地形for循环///	

					}//射线与地形相交////

					
				}  //绘制纹理////
			}//左键按下//
		if (mscene->bTerrainCreated)
		{
			mscene->mTerrainGroup->update(true);
		}
			
	}//编辑模式///
	
	return true;
}


bool InputManager::mouseMove( float dx,float dy,float dz )
{
	BaseApplication::getInstance()->mWindow->getMetrics(w,h,c,l,t);
	CPoint point;
	GetCursorPos(&point);
	newMousePos=Ogre::Vector3(point.x,point.y,0);
	dx=newMousePos.x-oldMousePos.x;
	dy=newMousePos.y-oldMousePos.y;
	oldMousePos=newMousePos;
	if (viewMode==1)
	{
		if (point.x>l&&point.x<(w+l)&&point.y>t&&point.y<(h+t))
		{
			if (dz>0)
			{
				mscene->cameraNode->translate(mouseZViewSpeed*mscene->mCamera->getDirection(), Ogre::Node::TS_LOCAL);    //调节相机远近
			}
			else if (dz<0)
			{
				mscene->cameraNode->translate(-mouseZViewSpeed*mscene->mCamera->getDirection(), Ogre::Node::TS_LOCAL);
			}
			if (pressedMB == 2)
			{
				mscene->cameraNode->pitch(Ogre::Degree(-dy/2.0f), Ogre::Node::TS_LOCAL);
				mscene->cameraNode->yaw(Ogre::Degree(-dx/2.0f), Ogre::Node::TS_WORLD );
			}	
			else if (pressedMB == 1)
			{
				if (mscene->mObjectEditMode==ObjMove||mscene->mObjectEditMode==ObjRoll||mscene->mObjectEditMode==ObjScale)
				{
					if (mEditPointor->bDruging==false&&mEditPointor->SelectState!=0&&pressedMB==1)
					{
						Ogre::Vector3 v3=Ogre::Vector3::NEGATIVE_UNIT_Z;
						Ogre::Vector3 camDirection=mscene->mCamera->getDerivedDirection();
						camDirection.y=0;			
						mEditPointor->camDegree=v3.angleBetween(Vector3(camDirection.x,0,camDirection.z));   //算出相机水平旋转角度,弧度自动转角度//	
						mEditPointor->bDruging=true;
					}
				}

			}


			if (mscene->mTerrainEditMode==AddObj)
			{
				
				if (mscene->bTerrainCreated)
				{
					Ogre::Ray mouseRay = mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));	
					TerrainGroup::RayResult rayResult =BaseApplication::getInstance()->mscene->mTerrainGroup->rayIntersects(mouseRay);
					if (rayResult.hit)
					{
						for(std::list<SceneObj*>::iterator i=mscene->tempAddObj.begin(); i!=mscene->tempAddObj.end();i++)
						{
							Vector3 oldPos=(*i)->mNode->getPosition();				
							Vector3 centerPos=(*i)->mEntity->getWorldBoundingBox(true).getCenter();
							Vector3 offset=centerPos-oldPos;   //算出模型的坐标原点与模型中心的偏离//
							Vector3 pos=rayResult.position;   //与地形的交点//
							pos.y+=(*i)->mEntity->getWorldBoundingBox(true).getHalfSize().y-offset.y;
							(*i)->setPosition(pos);
						}
					}	
				}
				else
				{
					Ogre::Ray mouseRay = mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));	
					Ogre::Plane waterPlane = Ogre::Plane(Ogre::Vector3::UNIT_Y,0);
					std::pair<bool,Ogre::Real> result = mouseRay.intersects(waterPlane);
					if (result.first)
					{
						for(std::list<SceneObj*>::iterator i=mscene->tempAddObj.begin(); i!=mscene->tempAddObj.end();i++)
						{
							Vector3 oldPos=(*i)->mNode->getPosition();				
							Vector3 centerPos=(*i)->mEntity->getWorldBoundingBox(true).getCenter();
							Vector3 offset=centerPos-oldPos;   //算出模型的坐标原点与模型中心的偏离//
							Vector3 pos=mouseRay.getPoint(result.second);
							pos.y+=(*i)->mEntity->getWorldBoundingBox(true).getHalfSize().y-offset.y;
							(*i)->setPosition(pos);
						}
					}
					
				}
				
			}
			
		}

		if (mscene->mObjectEditMode==ObjMove&&mscene->selectSceneObj.size()>0)
		{
			if (mEditPointor->bDruging)
			{
				Vector3 pos=mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->mPostion;
				if (mEditPointor->SelectState==1||mEditPointor->SelectState==4)
				{
					//向量夹角只能在0-180度，当相机的用方向向量的x值小于0时表示0-180度之间的角//
					//当相机的方向向量的x值大于0时表示180-360度之间的角//////////////////////
					if (mscene->mCamera->getDerivedDirection().x<=0)  
					{
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						pos.x+=(dx*cosValue+dy*sinValue) * mEditPointor->EditSpeed;		
					}
					else ////相机旋转角度在180-360范围/////
					{
						float degree=360-mEditPointor->camDegree.valueDegrees();
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						pos.x+=(dx*cosValue-dy*sinValue) * mEditPointor->EditSpeed;	
					}
				}
				if (mEditPointor->SelectState==2||mEditPointor->SelectState==4)
				{
					pos.y-=dy * mEditPointor->EditSpeed;
				}
				if (mEditPointor->SelectState==3||mEditPointor->SelectState==4)
				{
					if (mscene->mCamera->getDerivedDirection().x<=0)  
					{
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						pos.z+=(dy*cosValue-dx*sinValue) * mEditPointor->EditSpeed;		
					}
					else ////相机旋转角度在180-360范围/////
					{
						float degree=360-mEditPointor->camDegree.valueDegrees();
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						pos.z+=(dy*cosValue+dx*sinValue) * mEditPointor->EditSpeed;	
					}
				}
				mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->setPosition(pos);
			}
		}    //moveObj
		if (mscene->mObjectEditMode==ObjRoll&&mscene->selectSceneObj.size()>0)
		{
			if (mEditPointor->bDruging)
			{
				Vector3 rotation=mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->mRotaion;
				if (mEditPointor->SelectState==1)
				{
					rotation.y+=dx * mEditPointor->EditSpeed;			
					float f=rotation.y-(int)rotation.y;  //得到小数部分//
					rotation.y=(int)rotation.y%360;     //旋转角度对360整除取余//
					rotation.y+=f;       //加上小数部分//
				}
				if (mEditPointor->SelectState==2)
				{
					rotation.z-=dy * mEditPointor->EditSpeed;			
					float f=rotation.z-(int)rotation.z;  //得到小数部分//
					rotation.z=(int)rotation.z%360;     //旋转角度对360整除取余//
					rotation.z+=f;       //加上小数部分//
				}
				if (mEditPointor->SelectState==3)
				{
					rotation.x+=dy * mEditPointor->EditSpeed;			
					float f=rotation.x-(int)rotation.x;  //得到小数部分//
					rotation.x=(int)rotation.x%360;     //旋转角度对360整除取余//
					rotation.x+=f;       //加上小数部分//
				}
				mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->setRotaion(rotation);
			}
		} //rollObj//

		if (mscene->mObjectEditMode==ObjScale&&mscene->selectSceneObj.size()>0)
		{
			if (mEditPointor->bDruging)
			{
				Vector3 scale=mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->mScale;
				if (mEditPointor->SelectState==1||mEditPointor->SelectState==4)
				{
					//向量夹角只能在0-180度，当相机的用方向向量的x值小于0时表示0-180度之间的角//
					//当相机的方向向量的x值大于0时表示180-360度之间的角//////////////////////
					if (mscene->mCamera->getDerivedDirection().x<=0)  
					{
						float cosValue = Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue = Math::Sin(mEditPointor->camDegree.valueRadians());
						scale.x += (dx*cosValue+dy*sinValue) * mEditPointor->EditSpeed * 0.02;   //乘以0.02,减慢缩放速度//	
					}
					else ////相机旋转角度在180-360范围/////
					{
						float degree=360-mEditPointor->camDegree.valueDegrees();
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						scale.x+=(dx*cosValue-dy*sinValue) * mEditPointor->EditSpeed * 0.02;	
					}
				}
				if (mEditPointor->SelectState==2||mEditPointor->SelectState==4)
				{
					scale.y -= dy * mEditPointor->EditSpeed * 0.02;	
				}
				if (mEditPointor->SelectState==3||mEditPointor->SelectState==4)
				{
					if (mscene->mCamera->getDerivedDirection().x<=0)  
					{
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						scale.z+=(dy*cosValue-dx*sinValue) * mEditPointor->EditSpeed * 0.02;		
					}
					else ////相机旋转角度在180-360范围/////
					{
						float degree=360-mEditPointor->camDegree.valueDegrees();
						float cosValue=Math::Cos(mEditPointor->camDegree.valueRadians());
						float sinValue=Math::Sin(mEditPointor->camDegree.valueRadians());
						scale.z+=(dy*cosValue+dx*sinValue) * mEditPointor->EditSpeed * 0.02;	
					}
				}
				mscene->selectSceneObj.at(mscene->selectSceneObj.size()-1)->setScale(scale);
			}
		}//scale//
	}//viewMode==1//



	if (viewMode==2&&mHero!=NULL)
	{
		mHero->updateCameraGoal(-0.05f *dx, -0.05f * dy, -0.001f * dz);
	}
	return true;
}

bool InputManager::mousePress( int MBtnid )
{
	CPoint point;
	GetCursorPos(&point);
	if (point.x>l&&point.x<(w+l)&&point.y>t&&point.y<(h+t))
	{
		pressedMB=MBtnid;
	}
	else pressedMB=0;
	

	oldMousePos=Ogre::Vector3(point.x,point.y,0);
	if (viewMode==1 && mEditPointor!=NULL)
	{
		if (mscene->mTerrainEditMode==TerrainSelectObj&&mEditPointor->bDruging==false&&pressedMB==1)
		{
			CPoint point;	
			GetCursorPos(&point);
			if (point.x>l&&point.x<(w+l)&&point.y>t&&point.y<(h+t))
			{
				Ogre::Ray mouseRay =mscene->mCamera->getCameraToViewportRay((point.x-l)/float(w), (point.y-t)/float(h));
				mscene->mRaySceneQuery->setRay(mouseRay);
				mscene->mRaySceneQuery->setSortByDistance(true);    //设置按深度排序，方便得到第一个相交物体/////////
				RaySceneQueryResult &result =mscene->mRaySceneQuery->execute();
				RaySceneQueryResult::iterator itr = result.begin( );
				for (;itr!=result.end();++itr)
				{
					Ogre::String name=itr->movable->getParentNode()->getName();
					Ogre::String subName=name.substr(0,4);
					if (subName =="Mesh"||subName=="Soun"||subName=="Phys"||subName=="Ligh"||subName=="Part")
					{
						if (PressedKey&0x0100)   //control is down
						{
							for (int i=0;i<mscene->selectSceneObj.size();i++)
							{
								if (name==mscene->selectSceneObj.at(i)->mName)  //说明已经被选中，不可重复选择//
								{
									return true;    
								}
							}
							CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
							SceneObj *Obj=NULL;		HTREEITEM hitem=NULL;
							if (subName=="Mesh")
							{
								Obj = mscene->findSceneObjByName(name,0);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hMesh);
							}
							else if (subName=="Soun")
							{
								Obj=mscene->findSceneObjByName(name,1);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hSound);
							}
							else if (subName=="Phys")
							{
								CString Cname(name.c_str());
								int pos=Cname.ReverseFind('X');
								if (pos>6)
								{
									CString CSubname=Cname.Left(pos);
									USES_CONVERSION;         //CString转换std::string字符串要用此宏//
									std::string ostr = W2A(CSubname.GetBuffer());
									Ogre::String OName(ostr);
									Obj=mscene->findSceneObjByName(OName,2);
								}
								else { Obj=mscene->findSceneObjByName(name,2); }
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hSurroundBox);
							}
							else if (subName=="Ligh")
							{
								Obj=mscene->findSceneObjByName(name,3);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hLight);
							}
							else if (subName=="Part")
							{
								Obj=mscene->findSceneObjByName(name,4);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hParticle);
							}

							if (NULL!=Obj)
							{ 
								if (Obj->bVisualable)
								{
									while(hitem!=NULL)
									{
										CString str = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetItemText(hitem);
										USES_CONVERSION;         //CString转换std::string字符串要用此宏//
										std::string str1 = W2A(str.GetBuffer());
										Ogre::String ObjName(str1);
										if (ObjName==Obj->mName)
										{
											if (mscene->selectMode==0 && Obj->mObjType==PhysicsBoxObj)
											{
												pMain->m_wndSceneObjView.m_wndSceneObjTree.SelectItem(hitem);
												pMain->m_wndSceneObjView.m_wndSceneObjTree.EnsureVisible(hitem);
												pMain->m_wndObjProperties.InitPropList(Obj);
												return true;
											}
											if (mscene->selectMode==1 && Obj->mObjType!=PhysicsBoxObj)
											{
												pMain->m_wndSceneObjView.m_wndSceneObjTree.SelectItem(hitem);
												pMain->m_wndSceneObjView.m_wndSceneObjTree.EnsureVisible(hitem);
												pMain->m_wndObjProperties.InitPropList(Obj);
												return true;
											}
										}
										hitem= pMain->m_wndSceneObjView.m_wndSceneObjTree.GetNextSiblingItem(hitem);
									}	
								}
							}
							
						}
						else
						{		
							//itr->movable->getParentSceneNode()->showBoundingBox(true);
							SceneObj *Obj=NULL;  HTREEITEM hitem=NULL;
							CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
							if (subName =="Mesh")
							{
								Obj=mscene->findSceneObjByName(name,0);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hMesh);

							}
							if (subName=="Soun")
							{
								Obj=mscene->findSceneObjByName(name,1);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hSound);
							}
							if (subName=="Phys")
							{
								CString Cname(name.c_str());
								int pos=Cname.ReverseFind('X');
								if (pos>6)
								{
									CString CSubname=Cname.Left(pos);
									USES_CONVERSION;         //CString转换std::string字符串要用此宏//
									std::string ostr = W2A(CSubname.GetBuffer());
									Ogre::String OName(ostr);
									Obj=mscene->findSceneObjByName(OName,2);
								}
								else { Obj=mscene->findSceneObjByName(name,2); }
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hSurroundBox);
							}
							else if (subName=="Ligh")
							{
								Obj=mscene->findSceneObjByName(name,3);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hLight);
							}
							else if (subName=="Part")
							{
								Obj=mscene->findSceneObjByName(name,4);
								hitem = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetChildItem(pMain->m_wndSceneObjView.hParticle);
							}
							if (NULL!=Obj)
							{
								if (Obj->bVisualable)
								{
									while(hitem!=NULL)
									{
										CString str = pMain->m_wndSceneObjView.m_wndSceneObjTree.GetItemText(hitem);
										USES_CONVERSION;         //CString转换std::string字符串要用此宏//
										std::string str1 = W2A(str.GetBuffer());
										Ogre::String ObjName(str1);
										if (ObjName==Obj->mUserName)
										{
											if (mscene->selectMode==0 && Obj->mObjType==PhysicsBoxObj)
											{
												pMain->m_wndSceneObjView.m_wndSceneObjTree.SelectItem(hitem);
												pMain->m_wndSceneObjView.m_wndSceneObjTree.EnsureVisible(hitem);
												pMain->m_wndObjProperties.InitPropList(Obj);
												return true;
											}
											if (mscene->selectMode==1 && Obj->mObjType!=PhysicsBoxObj)
											{
												pMain->m_wndSceneObjView.m_wndSceneObjTree.SelectItem(hitem);
												pMain->m_wndSceneObjView.m_wndSceneObjTree.EnsureVisible(hitem);
												pMain->m_wndObjProperties.InitPropList(Obj);
												return true;
											}
										}
										hitem= pMain->m_wndSceneObjView.m_wndSceneObjTree.GetNextSiblingItem(hitem);
									}	
								}
							}			
						}

					}//subname//
				}//for
			}//pointer in  main window

		}//select object

	}

	if (viewMode==2&&mHero!=NULL)
	{
		if (mHero->mSwordsDrawn && (mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID == mHero->ANIM_RUN_TOP))
		{
			// if swords are out, and character's not doing something weird, then SLICE!
			if (MBtnid == 1) mHero->setTopAnimation(mHero->ANIM_SLICE_VERTICAL, true);
			else if (MBtnid == 2) mHero->setTopAnimation(mHero->ANIM_SLICE_HORIZONTAL, true);
			mHero->mTimer = 0;
		}
	}
	
	return true;
}

bool InputManager::mouseRelease( int MBtnid )
{
	pressedMB=MBtnid;

	if (viewMode==1)
	{
		if (mscene->mTerrainEditMode==AddObj&&mscene->tempAddObj.size()>0&&pressedMB==-1)
		{
			Ogre::String resname;
			std::list<SceneObj*>::iterator i=mscene->tempAddObj.begin();
			resname=(*i)->mResName;        //save the resname
		    mscene->AddSceneObj();
			mscene->createSceneObj(resname);
		}
		if (mscene->mTerrainEditMode==AddObj&&mscene->tempAddObj.size()>0&&pressedMB==-2)   //右键结束创建//
		{
			for (std::list<SceneObj*>::iterator i=mscene->tempAddObj.begin();i!=mscene->tempAddObj.end();++i)
			{
				if ((*i)->mObjType==MeshObj)
				{
					SceneMeshObj* obj=(SceneMeshObj*)*i;
					if (NULL != obj->mPhysicsBox)
					{
						PhysicsBox* phy=(PhysicsBox*)obj->mPhysicsBox;
						mscene->SceneNode->removeChild(phy->mNode);
					}
					
				}
				mscene->SceneNode->removeChild((*i)->mNode);   //remove temp from scene
				deletePtr(*i);   //删除临时对象，防止内存泄露///
			}
			mscene->tempAddObj.clear(); 
		}

		if (mEditPointor!=NULL)
		{
			if (mEditPointor->bDruging)
			{
				mEditPointor->bDruging=false;		
				//更新物体的属性显示//
				CMainFrame*  pMain=(CMainFrame*)AfxGetMainWnd();
				if (pMain->m_wndObjProperties.tempObj!=NULL)
				{
					SceneObj* sceneObj=(SceneObj*)pMain->m_wndObjProperties.tempObj;
					if (mscene->mObjectEditMode==ObjMove)
					{
						sceneObj->onUpdateValueChange(0);
					}
					if (mscene->mObjectEditMode==ObjScale)
					{
						sceneObj->onUpdateValueChange(1);
					}
					if (mscene->mObjectEditMode==ObjRoll)
					{
						sceneObj->onUpdateValueChange(2);
					}
				}
			}		
		}
	}
	


	return true;
}

void InputManager::changeViewMode()
{
	if (viewMode==1)
	{
		viewMode=2;
		mscene->mObjectEditMode=ObjNoneEdit;
		mscene->mTerrainEditMode=TerrainNoneEdit;
		mEditPointor->mPointorNode->setVisible(false);
		for(std::vector<SceneObj*>::iterator i=mscene->selectSceneObj.begin();i!=mscene->selectSceneObj.end();i++)
		{
			(*i)->showBoundingBox(false);
		}					
		mscene->selectSceneObj.clear();

		if (mHero==NULL)
		{
			mHero=Hero::getInstance();
		}
		BaseApplication::getInstance()->mRoot->addFrameListener(mHero);
		BaseApplication::getInstance()->mSceneMgr->getCamera("PlayerCam")->setFarClipDistance(10000);
		return;
	}
	if (viewMode==2)
	{
		viewMode=1;
		BaseApplication::getInstance()->mSceneMgr->getCamera("PlayerCam")->setFarClipDistance(20000);
		mEditPointor->mPointorNode->setVisible(true);
		BaseApplication::getInstance()->mRoot->removeFrameListener(mHero);
		return;
	}
}

void InputManager::keyPressed( MSG* pMsg)
{
	int key=pMsg->wParam;
	if (key=='L'&&mscene->bTerrainCreated)
	{
		changeViewMode();
	}

		if(pMsg->wParam=='W') {PressedKey |= (1<<0); }
		else if(pMsg->wParam=='S') {PressedKey |= (1<<1); }
		else if(pMsg->wParam=='A') {PressedKey |= (1<<2); }
		else if(pMsg->wParam=='D') {PressedKey |= (1<<3); }
		else if(pMsg->wParam==VK_UP) {PressedKey |= (1<<4); }
		else if(pMsg->wParam==VK_DOWN) {PressedKey |= (1<<5); }
		else if(pMsg->wParam==VK_LEFT) {PressedKey |= (1<<6); }
		else if(pMsg->wParam==VK_RIGHT) {PressedKey |= (1<<7); }
		else if (pMsg->wParam==VK_CONTROL){PressedKey |= (1<<8); }
		else if(pMsg->wParam=='Q') {mscene->mTerrainEditMode=TerrainNoneEdit; }
		else if(pMsg->wParam=='U') {mscene->mTerrainEditMode=TerrainUp; }
		else if(pMsg->wParam=='I') {mscene->mTerrainEditMode=TerrainDown; }
		else if(pMsg->wParam==VK_F1) {mscene->mSplatImage=0; }
		else if(pMsg->wParam==VK_F2) {mscene->mSplatImage=1; }
		else if(pMsg->wParam==VK_F3) {mscene->mSplatImage=2; }
		else if (pMsg->wParam=='P')
		{
			Ogre::StringVector *textures = new Ogre::StringVector();
			textures->push_back("growth_weirdfungus-03_diffusespecular.dds");
			TerrainGroup::TerrainIterator ti = mscene->mTerrainGroup->getTerrainIterator();
			while(ti.hasMoreElements())
			{
				Terrain* t = ti.getNext()->instance;
				int n = t->getLayerCount();
				t->removeLayer(n-1);
				t->addLayer(0,100,textures);
				t->updateCompositeMap();
				t->dirty();
				t->update();
				//Ogre::TerrainLayerDeclaration de = t->getLayerDeclaration();				
				//t->replaceLayer(t->getLayerCount()-1,true,300,textures);
			}
		}
		else if (pMsg->wParam=='K')
		{
			//设置地形保存路径和文件名,不带后缀////
			mscene->mTerrainGroup->setFilenamePrefix("C:\\Users\\Administrator\\Desktop\\boss\\Myterrain");
			mscene->mTerrainGroup->saveAllTerrains(false);
		}
		else if (pMsg->wParam=='L')
		{
			mscene->loadTerrain();
		}

	if (viewMode==2&&mHero!=NULL)
	{
		if ((key=='Q')&& (mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID == mHero->ANIM_RUN_TOP))
		{
			// take swords out (or put them back, since it's the same animation but reversed)
			mHero->setTopAnimation(mHero->ANIM_DRAW_SWORDS, true);
			mHero->mTimer = 0;
		}
		else if ((key=='E')&& !mHero->mSwordsDrawn)
		{
			if (mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID ==  mHero->ANIM_RUN_TOP)
			{
				// start dancing
				mHero->setBaseAnimation( mHero->ANIM_DANCE, true);
				mHero->setTopAnimation( mHero->ANIM_NONE);
				// disable hand animation because the dance controls hands
				mHero->mAnims[mHero->ANIM_HANDS_RELAXED]->setEnabled(false);
			}
			else if (mHero->mBaseAnimID == mHero->ANIM_DANCE)
			{
				// stop dancing
				mHero->setBaseAnimation(mHero->ANIM_IDLE_BASE);
				mHero->setTopAnimation(mHero->ANIM_IDLE_TOP);
				// re-enable hand animation
				mHero->mAnims[mHero->ANIM_HANDS_RELAXED]->setEnabled(true);
			}
		}

		// keep track of the player's intended direction
		else if (key=='W') mHero->mKeyDirection.z = -1;    //w
		else if (key=='S') mHero->mKeyDirection.z = 1;     //s
		else if (key=='A') mHero->mKeyDirection.x = -1;	//a
		else if (key=='D') mHero->mKeyDirection.x = 1;		//d

		else if (PressedKey&0x0010) mHero->mKeyDirection.z = -1;    //up
		else if (PressedKey&0x0020) mHero->mKeyDirection.z = 1;		//down
		else if (PressedKey&0x0040) mHero->mKeyDirection.x = -1;	//left
		else if (PressedKey&0x0080) mHero->mKeyDirection.x = 1;		//right

		else if ((PressedKey&0x8000) && (mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID == mHero->ANIM_RUN_TOP))
		{
			// jump if on ground
			mHero->setBaseAnimation(mHero->ANIM_JUMP_START, true);
			mHero->setTopAnimation(mHero->ANIM_NONE);
			mHero->mTimer = 0;
		}
		else if (key=='J'&&mHero->mSwordsDrawn&&(mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID == mHero->ANIM_RUN_TOP))
		{
			mHero->setTopAnimation(mHero->ANIM_SLICE_VERTICAL, true);
			mHero->mTimer = 0;
		}
		else if (key=='K'&&mHero->mSwordsDrawn&&(mHero->mTopAnimID == mHero->ANIM_IDLE_TOP || mHero->mTopAnimID == mHero->ANIM_RUN_TOP))
		{
			mHero->setTopAnimation(mHero->ANIM_SLICE_HORIZONTAL, true);
			mHero->mTimer = 0;
		}

		else if (key=='F') mHero->IsupdateCamera=1;	
		else if (key=='H') mHero->IsupdateCamera=2;	
		else if (key=='R') mHero->IsupdateCamera=3;
		else if (key=='V') mHero->IsupdateCamera=4;
		else if (key==0) mHero->IsupdateCamera=1;
		else if(pMsg->wParam==VK_RCONTROL) mHero->IsupdateCamera=2;

		if (!mHero->mKeyDirection.isZeroLength() && mHero->mBaseAnimID == mHero->ANIM_IDLE_BASE)
		{
			// start running if not already moving and the player wants to move
			mHero->setBaseAnimation(mHero->ANIM_RUN_BASE, true);
			if (mHero->mTopAnimID == mHero->ANIM_IDLE_TOP) mHero->setTopAnimation(mHero->ANIM_RUN_TOP, true);
		}

	}
}

void InputManager::keyReleased(MSG* pMsg)
{
	if(pMsg->wParam=='W') { PressedKey &= 0xfffe; }
	else if(pMsg->wParam=='S') { PressedKey &= 0xfffd; }
	else if(pMsg->wParam=='A') { PressedKey &= 0xfffb; }
	else if(pMsg->wParam=='D') { PressedKey &= 0xfff7; }
	else if(pMsg->wParam==VK_UP) { PressedKey &= 0xffef; }
	else if(pMsg->wParam==VK_DOWN) { PressedKey &= 0xffdf; }
	else if(pMsg->wParam==VK_LEFT) { PressedKey &= 0xffbf; }
	else if(pMsg->wParam==VK_RIGHT) { PressedKey &= 0xff7f; }
	else if(pMsg->wParam==VK_CONTROL){ PressedKey &=0xfeff; }
	else if (pMsg->wParam == 'M')
	{
		dBodySetPosition(mscene->sphbody,0.0f,35.0f,10.0);
		dBodySetLinearVel(mscene->sphbody,0,0,0);
	}


	if (viewMode==2&&mHero!=NULL)
	{
		// keep track of the player's intended direction
		if (pMsg->wParam == 'W' && mHero->mKeyDirection.z == -1) mHero->mKeyDirection.z = 0;
		else if (pMsg->wParam == 'A' && mHero->mKeyDirection.x == -1) mHero->mKeyDirection.x = 0;
		else if (pMsg->wParam == 'S' && mHero->mKeyDirection.z == 1) mHero->mKeyDirection.z = 0;
		else if (pMsg->wParam == 'D' && mHero->mKeyDirection.x == 1) mHero->mKeyDirection.x = 0;
		
		else if (pMsg->wParam == VK_UP &&  mHero->mKeyDirection.z == -1)  mHero->mKeyDirection.z = 0;
		else if (pMsg->wParam == VK_LEFT &&  mHero->mKeyDirection.x == -1)  mHero->mKeyDirection.x = 0;
		else if (pMsg->wParam == VK_DOWN &&  mHero->mKeyDirection.z == 1)  mHero->mKeyDirection.z = 0;
		else if (pMsg->wParam == VK_RIGHT && mHero-> mKeyDirection.x == 1)  mHero->mKeyDirection.x = 0;

		else if (pMsg->wParam == 'F'||pMsg->wParam == 'H'||pMsg->wParam == 'R'||pMsg->wParam == 'V'||pMsg->wParam == VK_NUMPAD0)
		{
			mHero->IsupdateCamera=0;
		}


		if (mHero->mKeyDirection.isZeroLength() && mHero->mBaseAnimID == mHero->ANIM_RUN_BASE)
		{
			// stop running if already moving and the player doesn't want to move
			mHero->setBaseAnimation(mHero->ANIM_IDLE_BASE);
			if (mHero->mTopAnimID == mHero->ANIM_RUN_TOP) mHero->setTopAnimation(mHero->ANIM_IDLE_TOP);
		}
	}

}

void InputManager::upDateSoundSystem(float time)
{
	if (viewMode==1 && mscene->bTerrainCreated)
	{

		//更新声音收听者的位置,这里与摄像机的位置绑定//////////////
		// vel = how far we moved last FRAME (m/f), then time compensate it to SECONDS (m/s)//
		FMOD_VECTOR vel;
		vel.x = (mscene->cameraNode->getPosition().x - mscene->ListenerLastPos.x) * time / 10;
		vel.y = (mscene->cameraNode->getPosition().y - mscene->ListenerLastPos.y) * time / 10;
		vel.z = (mscene->cameraNode->getPosition().z - mscene->ListenerLastPos.z) * time / 10;
		FMOD_VECTOR listenerpos={mscene->cameraNode->getPosition().x, mscene->cameraNode->getPosition().y, mscene->cameraNode->getPosition().z};
		// store pos for next time
		mscene->ListenerLastPos=mscene->cameraNode->getPosition();

		//FMOD_VECTOR forward        = {mCamera->getDirection().x, mCamera->getDirection().y, mCamera->getDirection().z };
		FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
		mscene->Fmodsystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
		mscene->soundUpdateTime+=time;
		if (mscene->soundUpdateTime>=0.05)
		{
			mscene->Fmodsystem->update();
			mscene->soundUpdateTime=0;
		}
	}
	if (viewMode==2 && mscene->bTerrainCreated && NULL!=mHero)
	{
		FMOD_VECTOR vel;
		vel.x = (mHero->mNewPos.x - mscene->ListenerLastPos.x) / time / 10;
		vel.y = (mHero->mNewPos.y - mscene->ListenerLastPos.y) / time / 10;
		vel.z = (mHero->mNewPos.z - mscene->ListenerLastPos.z) / time / 10;
		FMOD_VECTOR listenerpos={mHero->mNewPos.x, mHero->mNewPos.y,mHero->mNewPos.z};
		// store pos for next time
		mscene->ListenerLastPos=mHero->mNewPos;

		//FMOD_VECTOR forward        = {mCamera->getDirection().x, mCamera->getDirection().y, mCamera->getDirection().z };
		FMOD_VECTOR forward        = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up             = { 0.0f, 1.0f, 0.0f };
		mscene->Fmodsystem->set3DListenerAttributes(0, &listenerpos, &vel, &forward, &up);
		mscene->soundUpdateTime+=time;
		if (mscene->soundUpdateTime>=0.05)
		{
			mscene->Fmodsystem->update();
			mscene->soundUpdateTime=0;
		}
	}

}





