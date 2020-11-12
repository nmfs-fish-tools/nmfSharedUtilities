
#include "nmfChartLegend.h"
#include <nmfConstants.h>
#include <nmfConstantsMSSPM.h>


nmfChartLegend::nmfChartLegend(QChart *chart)
{
    m_chart = chart;
    m_tooltips.clear();
    m_markerColors.clear();
    m_maxNumMarkers = 0;
}

void
nmfChartLegend::setToolTips(std::vector<QString>& markerColors)
{
    m_markerColors = markerColors;
    m_maxNumMarkers = m_markerColors.size();
}


void
nmfChartLegend::setConnections()
{
    // Add hovered callbacks to set tooltips
    QList<QLegendMarker* > legendMarkers = m_chart->legend()->markers();
    int i=0;
    for (QLegendMarker* legendMarker : legendMarkers) {
        legendMarker->setObjectName(QString::number(i++));
        disconnect(legendMarker,0,0,0);
        connect(legendMarker, SIGNAL(hovered(bool)),
                this,         SLOT(callback_hoveredLegend(bool)));
    }
}

void
nmfChartLegend::callback_hoveredLegend(bool hovered)
{
    if (hovered) {
        QLegendMarker* marker = qobject_cast<QLegendMarker* >(QObject::sender());
        int markerIndex = marker->objectName().toInt();
        QString tooltip = m_markerColors[markerIndex % m_maxNumMarkers];
        QToolTip::showText(QCursor::pos(), tooltip);
    } else {
        QToolTip::hideText();
    }
}
