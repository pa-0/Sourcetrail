#include "QtGraphNodeComponent.h"

QtGraphNodeComponent::QtGraphNodeComponent(QtGraphNode* graphNode): m_graphNode(graphNode) {}

QtGraphNodeComponent::~QtGraphNodeComponent() = default;

void QtGraphNodeComponent::nodeMousePressEvent(QGraphicsSceneMouseEvent*  /*event*/) {}

void QtGraphNodeComponent::nodeMouseMoveEvent(QGraphicsSceneMouseEvent*  /*event*/) {}

void QtGraphNodeComponent::nodeMouseReleaseEvent(QGraphicsSceneMouseEvent*  /*event*/) {}
