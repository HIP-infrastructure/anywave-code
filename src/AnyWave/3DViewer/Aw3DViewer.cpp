// AnyWave
// Copyright (C) 2013-2021  INS UMR 1106
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include "Aw3DViewer.h"
#include "IO/BIDS/AwBIDSManager.h"
#include <Qt3DExtras>

// 3D Orientation
// X+ -> right
// Y+ -> away
// Z+ -> up


Aw3DViewer* Aw3DViewer::m_instance = nullptr;


Aw3DViewer::Aw3DViewer(QObject* parent) : AwDataClient(parent)
{
	m_rootEntity = nullptr;
	m_widget = nullptr;
}

Aw3DViewer::~Aw3DViewer()
{
}



int Aw3DViewer::init()
{
	if (AwBIDSManager::isInstantiated()) {
		if (AwBIDSManager::instance()->isBIDSActive()) {
			// a file must be open in bids
			auto dir = AwBIDSManager::instance()->getDerivativesPath(AwBIDSManager::freesurfer);
			if (dir.isEmpty())
				return;
			if (dir != m_freesurferDir) {
				m_freesurferDir = dir;
				start();
			}
		}
	}
}

void Aw3DViewer::start()
{
	if (m_rootEntity == nullptr)
		createScene();

}

Qt3DCore::QEntity* Aw3DViewer::createScene()
{
	m_rootEntity = new Qt3DCore::QEntity();
	Qt3DExtras::Qt3DWindow* view = new Qt3DExtras::Qt3DWindow();
	view->setRootEntity(m_rootEntity);
	m_widget = QWidget::createWindowContainer(view);
	// assign parameters to default camera
	Qt3DRender::QCamera* cameraEntity = view->camera();
	cameraEntity->lens()->setPerspectiveProjection(
		45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
	cameraEntity->setPosition(QVector3D(0, -5, 5.0f));
	cameraEntity->setUpVector(QVector3D(0, 0, 1));
	cameraEntity->setViewCenter(QVector3D(0, 0, 0));
	// add orbiting camera controller
	Qt3DExtras::QOrbitCameraController* camController = new
		Qt3DExtras::QOrbitCameraController(m_rootEntity);
	camController->setCamera(cameraEntity);
	// create ligth
	Qt3DCore::QEntity* lightEntity = new Qt3DCore::QEntity(m_rootEntity);
	Qt3DRender::QPointLight* light = new Qt3DRender::QPointLight(lightEntity);
	light->setColor("white");
	light->setIntensity(1);
	lightEntity->addComponent(light);
	 // After creating the light we must place it somewhere.This is achieved by adding a QTransform component to the light entity :
	Qt3DCore::QTransform* lightTransform = new Qt3DCore::QTransform(
		lightEntity);
	lightTransform->setTranslation(QVector3D(-20, 20, 20));
	lightEntity->addComponent(lightTransform);
	// load mesh here
}