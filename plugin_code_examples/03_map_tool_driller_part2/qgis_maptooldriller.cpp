#include "qgis_maptooldriller.h"

/*static*/ const QString Qgis_MapToolDrillerPLugin::s_name = QObject::tr("MapToolDrillerz");
/*static*/ const QString Qgis_MapToolDrillerPLugin::s_description = QObject::tr("Sample Plugin");
/*static*/ const QString Qgis_MapToolDrillerPLugin::s_category = QObject::tr("Plugins");
/*static*/ const QString Qgis_MapToolDrillerPLugin::s_version = QObject::tr("Version 0.1.1");
/*static*/ const QString Qgis_MapToolDrillerPLugin::s_icon = "";
/*static*/ const QgisPlugin::PLUGINTYPE Qgis_MapToolDrillerPLugin::s_type = QgisPlugin::UI;

// for logging
/*static*/ const QString Qgis_MapToolDrillerPLugin::mTag = QObject::tr("MapToolDriller");

QGISEXTERN QgisPlugin* classFactory(QgisInterface* qgis_if)
{
   std::cout << "::classFactory" << std::endl;
   return new Qgis_MapToolDrillerPLugin(qgis_if);
}

QGISEXTERN QString name()
{
   std::cout << "::name" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_name;
}

QGISEXTERN QString description()
{
   std::cout << "::description" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_description;
}

QGISEXTERN QString category()
{
   std::cout << "::category" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_category;
}

QGISEXTERN int type()
{
   std::cout << "::type" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_type;
}

QGISEXTERN QString version()
{
   std::cout << "::version" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_version;
}

QGISEXTERN QString icon()
{
   std::cout << "::icon" << std::endl;
   return Qgis_MapToolDrillerPLugin::s_icon;
}

QGISEXTERN void unload(QgisPlugin* plugin)
{
   std::cout << "::unload" << std::endl;
   delete plugin;
}

/*virtual*/ Qgis_MapToolDrillerPLugin::~Qgis_MapToolDrillerPLugin()
{

}


/*virtual*/ void Qgis_MapToolDrillerPLugin::initGui()
{
    std::cout << "Qgis_MapToolDrillerPLugin::initGui" << std::endl;

    // add an action to the menu
    m_action = new QAction(QIcon("" ), tr("MapToolDriller"), this);
    m_action->setWhatsThis(tr("Click on the map canvas like never before"));
    connect(m_action, SIGNAL(triggered()), this, SLOT(StartOverlay()));

    m_qgis_if->addRasterToolBarIcon(m_action);
    m_qgis_if->addPluginToMenu(tr("&MapToolDriller"), m_action);

    // create our map tool
    mpMapTool = new MapToolDriller( m_qgis_if->mapCanvas() );
    connect( mpMapTool, SIGNAL( mouseMoved( QgsPoint ) ), this, SLOT( mouseMoved( QgsPoint ) ) );
    connect( mpMapTool, SIGNAL( mouseClicked( QgsPoint ) ), this, SLOT( mouseClicked( QgsPoint ) ) );
}


void Qgis_MapToolDrillerPLugin::mouseClicked( QgsPoint thePoint )
{
    QString mFeedback = thePoint.toString();
    QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
}
void Qgis_MapToolDrillerPLugin::mouseMoved( QgsPoint thePoint )
{
    QString mFeedback = thePoint.toString();
    QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
}

void Qgis_MapToolDrillerPLugin::StartOverlay()
{
    std::cout << "Qgis_MapToolDrillerPLugin::StartOverlay" << std::endl;

    // Get the map canvas
    QgsMapCanvas* canvas = m_qgis_if->mapCanvas();
    // set the tool
    canvas->setMapTool(mpMapTool);

    // Layer QStrings
    QString mLayerPath         = "/home/junkmob/QGIS-DEV/QGIS/tests/testdata/points.shp";
    QString mLayerBaseName     = "points";
    QString mProviderName      = "ogr";

    // Get Vector Layer
    QgsVectorLayer * mLayer = new QgsVectorLayer(mLayerPath, mLayerBaseName, mProviderName);
    if (mLayer->isValid())
    {
        QString mFeedback = "layer is valid";
        QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
    }
    else
    {
        QString mFeedback = "layer NOT valid";
        QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
    }

    // Add the Vector Layer to the Layer Registry
    QgsMapLayerRegistry::instance()->addMapLayer(mLayer, TRUE);

    // Add the Layer to the Layer Set
    QList <QgsMapCanvasLayer> mLayerSet;
    mLayerSet.append(QgsMapCanvasLayer(mLayer, TRUE));

	// Create the Map Canvas
	canvas->setExtent(mLayer->extent());
	canvas->enableAntiAliasing(true);
	canvas->setCanvasColor(QColor(255, 255, 255));
	canvas->freeze(false);

	// add it to the canvas
	canvas->setLayerSet( mLayerSet );
	canvas->setVisible(true);
	canvas->refresh();

}

