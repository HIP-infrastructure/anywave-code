/****************************************************************************
** Meta object code from reading C++ file 'AwGraphicsScene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../../../../include/widget/SignalView/AwGraphicsScene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AwGraphicsScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AwGraphicsScene_t {
    QByteArrayData data[98];
    char stringdata0[1374];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AwGraphicsScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AwGraphicsScene_t qt_meta_stringdata_AwGraphicsScene = {
    {
QT_MOC_LITERAL(0, 0, 15), // "AwGraphicsScene"
QT_MOC_LITERAL(1, 16, 13), // "clickedAtTime"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 4), // "time"
QT_MOC_LITERAL(4, 36, 32), // "numberOfDisplayedChannelsChanged"
QT_MOC_LITERAL(5, 69, 6), // "number"
QT_MOC_LITERAL(6, 76, 11), // "needRefresh"
QT_MOC_LITERAL(7, 88, 17), // "newSceneSelection"
QT_MOC_LITERAL(8, 106, 14), // "AwChannelList&"
QT_MOC_LITERAL(9, 121, 8), // "channels"
QT_MOC_LITERAL(10, 130, 15), // "markersSelected"
QT_MOC_LITERAL(11, 146, 13), // "AwMarkerList&"
QT_MOC_LITERAL(12, 160, 7), // "markers"
QT_MOC_LITERAL(13, 168, 20), // "updatePositionInFile"
QT_MOC_LITERAL(14, 189, 8), // "position"
QT_MOC_LITERAL(15, 198, 21), // "channelFiltersChanged"
QT_MOC_LITERAL(16, 220, 21), // "cursorPositionChanged"
QT_MOC_LITERAL(17, 242, 22), // "mappingPositionChanged"
QT_MOC_LITERAL(18, 265, 14), // "markerInserted"
QT_MOC_LITERAL(19, 280, 9), // "AwMarker*"
QT_MOC_LITERAL(20, 290, 6), // "marker"
QT_MOC_LITERAL(21, 297, 19), // "cursorClickedAtTime"
QT_MOC_LITERAL(22, 317, 24), // "mappingTimeSelectionDone"
QT_MOC_LITERAL(23, 342, 8), // "duration"
QT_MOC_LITERAL(24, 351, 13), // "badChannelSet"
QT_MOC_LITERAL(25, 365, 5), // "label"
QT_MOC_LITERAL(26, 371, 24), // "processSelectedForLaunch"
QT_MOC_LITERAL(27, 396, 8), // "QString&"
QT_MOC_LITERAL(28, 405, 4), // "name"
QT_MOC_LITERAL(29, 410, 3), // "pos"
QT_MOC_LITERAL(30, 414, 3), // "end"
QT_MOC_LITERAL(31, 418, 12), // "QTSModeEnded"
QT_MOC_LITERAL(32, 431, 16), // "closeViewClicked"
QT_MOC_LITERAL(33, 448, 21), // "registerDisplayPlugin"
QT_MOC_LITERAL(34, 470, 16), // "AwDisplayPlugin*"
QT_MOC_LITERAL(35, 487, 6), // "plugin"
QT_MOC_LITERAL(36, 494, 14), // "updateSettings"
QT_MOC_LITERAL(37, 509, 15), // "AwViewSettings*"
QT_MOC_LITERAL(38, 525, 8), // "settings"
QT_MOC_LITERAL(39, 534, 5), // "flags"
QT_MOC_LITERAL(40, 540, 18), // "setMarkingSettings"
QT_MOC_LITERAL(41, 559, 18), // "AwMarkingSettings*"
QT_MOC_LITERAL(42, 578, 15), // "setPageDuration"
QT_MOC_LITERAL(43, 594, 3), // "dur"
QT_MOC_LITERAL(44, 598, 16), // "setStartPosition"
QT_MOC_LITERAL(45, 615, 15), // "updateSelection"
QT_MOC_LITERAL(46, 631, 27), // "displaySelectedChannelsOnly"
QT_MOC_LITERAL(47, 659, 18), // "displayAllChannels"
QT_MOC_LITERAL(48, 678, 8), // "nextPage"
QT_MOC_LITERAL(49, 687, 4), // "next"
QT_MOC_LITERAL(50, 692, 7), // "goToEnd"
QT_MOC_LITERAL(51, 700, 9), // "goToStart"
QT_MOC_LITERAL(52, 710, 12), // "previousPage"
QT_MOC_LITERAL(53, 723, 8), // "previous"
QT_MOC_LITERAL(54, 732, 18), // "updateChannelsData"
QT_MOC_LITERAL(55, 751, 11), // "goToLatency"
QT_MOC_LITERAL(56, 763, 20), // "centerViewOnPosition"
QT_MOC_LITERAL(57, 784, 17), // "highlightPosition"
QT_MOC_LITERAL(58, 802, 11), // "showMarkers"
QT_MOC_LITERAL(59, 814, 4), // "show"
QT_MOC_LITERAL(60, 819, 28), // "changeChannelsSelectionState"
QT_MOC_LITERAL(61, 848, 8), // "selected"
QT_MOC_LITERAL(62, 857, 17), // "selectAllChannels"
QT_MOC_LITERAL(63, 875, 22), // "invertChannelSelection"
QT_MOC_LITERAL(64, 898, 21), // "clearChannelSelection"
QT_MOC_LITERAL(65, 920, 20), // "selectChannelsOfType"
QT_MOC_LITERAL(66, 941, 4), // "type"
QT_MOC_LITERAL(67, 946, 14), // "selectChannels"
QT_MOC_LITERAL(68, 961, 6), // "labels"
QT_MOC_LITERAL(69, 968, 23), // "selectChannelAtPosition"
QT_MOC_LITERAL(70, 992, 11), // "showChannel"
QT_MOC_LITERAL(71, 1004, 10), // "setMarkers"
QT_MOC_LITERAL(72, 1015, 12), // "AwMarkerList"
QT_MOC_LITERAL(73, 1028, 18), // "addHighLigthMarker"
QT_MOC_LITERAL(74, 1047, 4), // "text"
QT_MOC_LITERAL(75, 1052, 21), // "removeHighLigthMarker"
QT_MOC_LITERAL(76, 1074, 15), // "highlightMarker"
QT_MOC_LITERAL(77, 1090, 1), // "m"
QT_MOC_LITERAL(78, 1092, 13), // "setCursorMode"
QT_MOC_LITERAL(79, 1106, 5), // "OnOff"
QT_MOC_LITERAL(80, 1112, 14), // "setMarkingMode"
QT_MOC_LITERAL(81, 1127, 4), // "flag"
QT_MOC_LITERAL(82, 1132, 14), // "setMappingMode"
QT_MOC_LITERAL(83, 1147, 2), // "on"
QT_MOC_LITERAL(84, 1150, 10), // "setQTSMode"
QT_MOC_LITERAL(85, 1161, 17), // "setCursorPosition"
QT_MOC_LITERAL(86, 1179, 9), // "secsPerCm"
QT_MOC_LITERAL(87, 1189, 24), // "setMappingCursorPosition"
QT_MOC_LITERAL(88, 1214, 25), // "removeLastMappingPosition"
QT_MOC_LITERAL(89, 1240, 21), // "selectUnselectChannel"
QT_MOC_LITERAL(90, 1262, 15), // "showHideMarkers"
QT_MOC_LITERAL(91, 1278, 14), // "cursorToMarker"
QT_MOC_LITERAL(92, 1293, 23), // "addCustomMarkerFromList"
QT_MOC_LITERAL(93, 1317, 15), // "setChannelAsBad"
QT_MOC_LITERAL(94, 1333, 11), // "gotoChannel"
QT_MOC_LITERAL(95, 1345, 8), // "QAction*"
QT_MOC_LITERAL(96, 1354, 3), // "act"
QT_MOC_LITERAL(97, 1358, 15) // "launchQTSPlugin"

    },
    "AwGraphicsScene\0clickedAtTime\0\0time\0"
    "numberOfDisplayedChannelsChanged\0"
    "number\0needRefresh\0newSceneSelection\0"
    "AwChannelList&\0channels\0markersSelected\0"
    "AwMarkerList&\0markers\0updatePositionInFile\0"
    "position\0channelFiltersChanged\0"
    "cursorPositionChanged\0mappingPositionChanged\0"
    "markerInserted\0AwMarker*\0marker\0"
    "cursorClickedAtTime\0mappingTimeSelectionDone\0"
    "duration\0badChannelSet\0label\0"
    "processSelectedForLaunch\0QString&\0"
    "name\0pos\0end\0QTSModeEnded\0closeViewClicked\0"
    "registerDisplayPlugin\0AwDisplayPlugin*\0"
    "plugin\0updateSettings\0AwViewSettings*\0"
    "settings\0flags\0setMarkingSettings\0"
    "AwMarkingSettings*\0setPageDuration\0"
    "dur\0setStartPosition\0updateSelection\0"
    "displaySelectedChannelsOnly\0"
    "displayAllChannels\0nextPage\0next\0"
    "goToEnd\0goToStart\0previousPage\0previous\0"
    "updateChannelsData\0goToLatency\0"
    "centerViewOnPosition\0highlightPosition\0"
    "showMarkers\0show\0changeChannelsSelectionState\0"
    "selected\0selectAllChannels\0"
    "invertChannelSelection\0clearChannelSelection\0"
    "selectChannelsOfType\0type\0selectChannels\0"
    "labels\0selectChannelAtPosition\0"
    "showChannel\0setMarkers\0AwMarkerList\0"
    "addHighLigthMarker\0text\0removeHighLigthMarker\0"
    "highlightMarker\0m\0setCursorMode\0OnOff\0"
    "setMarkingMode\0flag\0setMappingMode\0"
    "on\0setQTSMode\0setCursorPosition\0"
    "secsPerCm\0setMappingCursorPosition\0"
    "removeLastMappingPosition\0"
    "selectUnselectChannel\0showHideMarkers\0"
    "cursorToMarker\0addCustomMarkerFromList\0"
    "setChannelAsBad\0gotoChannel\0QAction*\0"
    "act\0launchQTSPlugin"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AwGraphicsScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      63,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      16,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  329,    2, 0x06 /* Public */,
       4,    1,  332,    2, 0x06 /* Public */,
       6,    0,  335,    2, 0x06 /* Public */,
       7,    1,  336,    2, 0x06 /* Public */,
      10,    1,  339,    2, 0x06 /* Public */,
      13,    1,  342,    2, 0x06 /* Public */,
      15,    0,  345,    2, 0x06 /* Public */,
      16,    1,  346,    2, 0x06 /* Public */,
      17,    1,  349,    2, 0x06 /* Public */,
      18,    1,  352,    2, 0x06 /* Public */,
      21,    1,  355,    2, 0x06 /* Public */,
      22,    2,  358,    2, 0x06 /* Public */,
      24,    1,  363,    2, 0x06 /* Public */,
      26,    4,  366,    2, 0x06 /* Public */,
      31,    0,  375,    2, 0x06 /* Public */,
      32,    0,  376,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      33,    1,  377,    2, 0x0a /* Public */,
      36,    2,  380,    2, 0x0a /* Public */,
      40,    1,  385,    2, 0x0a /* Public */,
      42,    1,  388,    2, 0x0a /* Public */,
      44,    1,  391,    2, 0x0a /* Public */,
      45,    0,  394,    2, 0x0a /* Public */,
      46,    0,  395,    2, 0x0a /* Public */,
      47,    0,  396,    2, 0x0a /* Public */,
      48,    0,  397,    2, 0x0a /* Public */,
      49,    0,  398,    2, 0x0a /* Public */,
      50,    0,  399,    2, 0x0a /* Public */,
      51,    0,  400,    2, 0x0a /* Public */,
      52,    0,  401,    2, 0x0a /* Public */,
      53,    0,  402,    2, 0x0a /* Public */,
      54,    0,  403,    2, 0x0a /* Public */,
      55,    0,  404,    2, 0x0a /* Public */,
      56,    1,  405,    2, 0x0a /* Public */,
      57,    1,  408,    2, 0x0a /* Public */,
      58,    1,  411,    2, 0x0a /* Public */,
      60,    2,  414,    2, 0x0a /* Public */,
      62,    0,  419,    2, 0x0a /* Public */,
      63,    0,  420,    2, 0x0a /* Public */,
      64,    0,  421,    2, 0x0a /* Public */,
      65,    0,  422,    2, 0x0a /* Public */,
      65,    1,  423,    2, 0x0a /* Public */,
      67,    1,  426,    2, 0x0a /* Public */,
      69,    1,  429,    2, 0x0a /* Public */,
      70,    1,  432,    2, 0x0a /* Public */,
      71,    1,  435,    2, 0x0a /* Public */,
      73,    3,  438,    2, 0x0a /* Public */,
      75,    0,  445,    2, 0x0a /* Public */,
      76,    1,  446,    2, 0x0a /* Public */,
      78,    1,  449,    2, 0x0a /* Public */,
      80,    1,  452,    2, 0x0a /* Public */,
      82,    1,  455,    2, 0x0a /* Public */,
      84,    1,  458,    2, 0x0a /* Public */,
      85,    1,  461,    2, 0x0a /* Public */,
      85,    2,  464,    2, 0x0a /* Public */,
      87,    1,  469,    2, 0x0a /* Public */,
      88,    0,  472,    2, 0x0a /* Public */,
      89,    0,  473,    2, 0x09 /* Protected */,
      90,    0,  474,    2, 0x09 /* Protected */,
      91,    0,  475,    2, 0x09 /* Protected */,
      92,    0,  476,    2, 0x09 /* Protected */,
      93,    0,  477,    2, 0x09 /* Protected */,
      94,    1,  478,    2, 0x09 /* Protected */,
      97,    0,  481,    2, 0x09 /* Protected */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, 0x80000000 | 11,   12,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, 0x80000000 | 19,   20,
    QMetaType::Void, QMetaType::Float,    3,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,    3,   23,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, 0x80000000 | 27, 0x80000000 | 8, QMetaType::Float, QMetaType::Float,   28,    9,   29,   30,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 34,   35,
    QMetaType::Void, 0x80000000 | 37, QMetaType::Int,   38,   39,
    QMetaType::Void, 0x80000000 | 41,   38,
    QMetaType::Void, QMetaType::Float,   43,
    QMetaType::Void, QMetaType::Float,   29,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Float,   29,
    QMetaType::Void, QMetaType::Float,   29,
    QMetaType::Void, QMetaType::Bool,   59,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   28,   61,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   66,
    QMetaType::Void, QMetaType::QStringList,   68,
    QMetaType::Void, QMetaType::QPointF,   29,
    QMetaType::Void, QMetaType::QString,   25,
    QMetaType::Void, 0x80000000 | 72,   12,
    QMetaType::Void, QMetaType::QString, QMetaType::Float, QMetaType::Float,   74,   29,   43,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 19,   77,
    QMetaType::Void, QMetaType::Bool,   79,
    QMetaType::Void, QMetaType::Bool,   81,
    QMetaType::Void, QMetaType::Bool,   83,
    QMetaType::Void, QMetaType::Bool,   83,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void, QMetaType::Float, QMetaType::Float,   14,   86,
    QMetaType::Void, QMetaType::Float,   14,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 95,   96,
    QMetaType::Void,

       0        // eod
};

void AwGraphicsScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AwGraphicsScene *_t = static_cast<AwGraphicsScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->clickedAtTime((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: _t->numberOfDisplayedChannelsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->needRefresh(); break;
        case 3: _t->newSceneSelection((*reinterpret_cast< AwChannelList(*)>(_a[1]))); break;
        case 4: _t->markersSelected((*reinterpret_cast< AwMarkerList(*)>(_a[1]))); break;
        case 5: _t->updatePositionInFile((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 6: _t->channelFiltersChanged(); break;
        case 7: _t->cursorPositionChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 8: _t->mappingPositionChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: _t->markerInserted((*reinterpret_cast< AwMarker*(*)>(_a[1]))); break;
        case 10: _t->cursorClickedAtTime((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 11: _t->mappingTimeSelectionDone((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 12: _t->badChannelSet((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->processSelectedForLaunch((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< AwChannelList(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3])),(*reinterpret_cast< float(*)>(_a[4]))); break;
        case 14: _t->QTSModeEnded(); break;
        case 15: _t->closeViewClicked(); break;
        case 16: _t->registerDisplayPlugin((*reinterpret_cast< AwDisplayPlugin*(*)>(_a[1]))); break;
        case 17: _t->updateSettings((*reinterpret_cast< AwViewSettings*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 18: _t->setMarkingSettings((*reinterpret_cast< AwMarkingSettings*(*)>(_a[1]))); break;
        case 19: _t->setPageDuration((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 20: _t->setStartPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 21: _t->updateSelection(); break;
        case 22: _t->displaySelectedChannelsOnly(); break;
        case 23: _t->displayAllChannels(); break;
        case 24: _t->nextPage(); break;
        case 25: _t->next(); break;
        case 26: _t->goToEnd(); break;
        case 27: _t->goToStart(); break;
        case 28: _t->previousPage(); break;
        case 29: _t->previous(); break;
        case 30: _t->updateChannelsData(); break;
        case 31: _t->goToLatency(); break;
        case 32: _t->centerViewOnPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 33: _t->highlightPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 34: _t->showMarkers((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 35: _t->changeChannelsSelectionState((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 36: _t->selectAllChannels(); break;
        case 37: _t->invertChannelSelection(); break;
        case 38: _t->clearChannelSelection(); break;
        case 39: _t->selectChannelsOfType(); break;
        case 40: _t->selectChannelsOfType((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 41: _t->selectChannels((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 42: _t->selectChannelAtPosition((*reinterpret_cast< const QPointF(*)>(_a[1]))); break;
        case 43: _t->showChannel((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 44: _t->setMarkers((*reinterpret_cast< const AwMarkerList(*)>(_a[1]))); break;
        case 45: _t->addHighLigthMarker((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        case 46: _t->removeHighLigthMarker(); break;
        case 47: _t->highlightMarker((*reinterpret_cast< AwMarker*(*)>(_a[1]))); break;
        case 48: _t->setCursorMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 49: _t->setMarkingMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 50: _t->setMappingMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 51: _t->setQTSMode((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 52: _t->setCursorPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 53: _t->setCursorPosition((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2]))); break;
        case 54: _t->setMappingCursorPosition((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 55: _t->removeLastMappingPosition(); break;
        case 56: _t->selectUnselectChannel(); break;
        case 57: _t->showHideMarkers(); break;
        case 58: _t->cursorToMarker(); break;
        case 59: _t->addCustomMarkerFromList(); break;
        case 60: _t->setChannelAsBad(); break;
        case 61: _t->gotoChannel((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 62: _t->launchQTSPlugin(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 17:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwViewSettings* >(); break;
            }
            break;
        case 44:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< AwMarkerList >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AwGraphicsScene::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::clickedAtTime)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::numberOfDisplayedChannelsChanged)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::needRefresh)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(AwChannelList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::newSceneSelection)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(AwMarkerList & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::markersSelected)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::updatePositionInFile)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::channelFiltersChanged)) {
                *result = 6;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::cursorPositionChanged)) {
                *result = 7;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::mappingPositionChanged)) {
                *result = 8;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(AwMarker * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::markerInserted)) {
                *result = 9;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::cursorClickedAtTime)) {
                *result = 10;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::mappingTimeSelectionDone)) {
                *result = 11;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::badChannelSet)) {
                *result = 12;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)(QString & , AwChannelList & , float , float );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::processSelectedForLaunch)) {
                *result = 13;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::QTSModeEnded)) {
                *result = 14;
                return;
            }
        }
        {
            typedef void (AwGraphicsScene::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AwGraphicsScene::closeViewClicked)) {
                *result = 15;
                return;
            }
        }
    }
}

const QMetaObject AwGraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_AwGraphicsScene.data,
      qt_meta_data_AwGraphicsScene,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AwGraphicsScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AwGraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AwGraphicsScene.stringdata0))
        return static_cast<void*>(const_cast< AwGraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int AwGraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 63)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 63;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 63)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 63;
    }
    return _id;
}

// SIGNAL 0
void AwGraphicsScene::clickedAtTime(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AwGraphicsScene::numberOfDisplayedChannelsChanged(int _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AwGraphicsScene::needRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void AwGraphicsScene::newSceneSelection(AwChannelList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AwGraphicsScene::markersSelected(AwMarkerList & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void AwGraphicsScene::updatePositionInFile(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void AwGraphicsScene::channelFiltersChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 6, Q_NULLPTR);
}

// SIGNAL 7
void AwGraphicsScene::cursorPositionChanged(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void AwGraphicsScene::mappingPositionChanged(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void AwGraphicsScene::markerInserted(AwMarker * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void AwGraphicsScene::cursorClickedAtTime(float _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void AwGraphicsScene::mappingTimeSelectionDone(float _t1, float _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void AwGraphicsScene::badChannelSet(const QString & _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void AwGraphicsScene::processSelectedForLaunch(QString & _t1, AwChannelList & _t2, float _t3, float _t4)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void AwGraphicsScene::QTSModeEnded()
{
    QMetaObject::activate(this, &staticMetaObject, 14, Q_NULLPTR);
}

// SIGNAL 15
void AwGraphicsScene::closeViewClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 15, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
