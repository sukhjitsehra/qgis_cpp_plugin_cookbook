#include "qgis_maptooldriller.h"

#include <QAction>
#include <QToolBar>
#include <QDockWidget>
#include <QLayout>
#include <QLineEdit>
#include <QClipboard>
#include <QPushButton>
#include <QToolButton>
#include <QFile>
#include <QLabel>

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

    // create the dockable widget
    mpDockWidget = new QDockWidget( tr( "MapToolDriller Dockable" ), m_qgis_if->mainWindow() );
    mpDockWidget->setObjectName( "MapToolDriller" );
    mpDockWidget->setAllowedAreas( Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea );
    m_qgis_if->addDockWidget( Qt::LeftDockWidgetArea, mpDockWidget );

    // add an action to the menu
    m_action = new QAction(QIcon("" ), tr("MapToolDriller"), this);
    m_action->setCheckable( true );
    m_action->setChecked( mpDockWidget->isVisible() );
    m_action->setWhatsThis(tr("Click on the map canvas like never before"));
    connect(m_action, SIGNAL(triggered()), this, SLOT(StartOverlay()));
    m_qgis_if->addPluginToMenu(tr("&MapToolDriller"), m_action);
    m_qgis_if->addVectorToolBarIcon(m_action);

    // create our map tool
    mpMapTool = new MapToolDriller( m_qgis_if->mapCanvas() );
    connect( mpMapTool, SIGNAL( mouseMoved( QgsPoint ) ), this, SLOT( mouseMoved( QgsPoint ) ) );
    connect( mpMapTool, SIGNAL( mouseClicked( QgsPoint ) ), this, SLOT( mouseClicked( QgsPoint ) ) );

    // create the widget dialog
    QWidget * mypWidget = new QWidget();
    QGridLayout *mypLayout = new QGridLayout( mypWidget );
    mypLayout->setColumnMinimumWidth( 0, 36 );
    mypWidget->setLayout( mypLayout );

    mpOnButton = new QPushButton( mypWidget );
    mpOnButton->setText( tr("Drillerz Away") );
    mpOnButton->setToolTip( tr( "Click to start drillerzen away" ) );
    connect( mpOnButton, SIGNAL( clicked() ), this, SLOT( run() ) );

    mpFeedbackEdit = new QLineEdit( mypWidget );
    mpFeedbackEdit->setReadOnly( true );
    mpFeedbackEdit->setToolTip( tr( "Copy this stuff if you think it's useful" ) );

    mypLayout->addWidget( mpOnButton, 0, 0 );
    mypLayout->addWidget( mpFeedbackEdit, 1, 0 );

    // now add our custom widget to the dock - ownership of the widget is passed to the dock
    mpDockWidget->setWidget( mypWidget );
    connect( mpDockWidget, SIGNAL( visibilityChanged( bool ) ), m_action, SLOT( setChecked( bool ) ) );
}


void Qgis_MapToolDrillerPLugin::mouseClicked( QgsPoint thePoint )
{
    QString mFeedback = thePoint.toString();
    QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
    mpFeedbackEdit->setText( mFeedback );
}
void Qgis_MapToolDrillerPLugin::mouseMoved( QgsPoint thePoint )
{
    QString mFeedback = thePoint.toString();
    QgsMessageLog::instance()->logMessage( mFeedback, mTag, QgsMessageLog::INFO );
}

void Qgis_MapToolDrillerPLugin::run(){
    m_qgis_if->mapCanvas()->setMapTool( mpMapTool );
}

void Qgis_MapToolDrillerPLugin::help(){
    // implement me
}

void Qgis_MapToolDrillerPLugin::showOrHide(){
  if ( !mpDockWidget )
    run();
  else
    if ( m_action->isChecked() )
      mpDockWidget->show();
    else
      mpDockWidget->hide();
}


void Qgis_MapToolDrillerPLugin::StartOverlay()
{
    std::cout << "Qgis_MapToolDrillerPLugin::StartOverlay" << std::endl;

    // show or hide
    showOrHide();

    // Get the map canvas
    QgsMapCanvas* canvas = m_qgis_if->mapCanvas();

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

