// Fill out your copyright notice in the Description page of Project Settings.


#include "Slate/SSphereCurBox.h"
#include "Rendering/DrawElements.h"
#include "Types/SlateConstants.h"
#include "Layout/LayoutUtils.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/SOverlay.h"
#include "Framework/Application/SlateApplication.h"
#include "Widgets/Images/SImage.h"
#include "Layout/Children.h"

//根据控件折叠方向和控件某个坐标求得另一个坐标值
template<ESpherePosAlignment SpherePosAlignment>
FVector2D GetSpherePointPos(float XorY,bool IsEnabledSphere, FVector2D LoadSize, FVector2D WidgetSize, FVector2D WidgetSizeAlignment)
{	
	/*
	if (!IsEnabledSphere)
	{
		return SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right ? FVector2D(0, XorY) : FVector2D(XorY, 0);
	}
	if (XorY + (WidgetSize * (-WidgetSizeAlignment + 1)).Y > LoadSize.Y || XorY + (WidgetSize * ( - WidgetSizeAlignment + 1)).Y < 0)
	{
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left)
		{
			return FVector2D((LoadSize + WidgetSize).X / 2, XorY);
		}
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
		{
			return FVector2D((LoadSize - WidgetSize).X / 2, XorY);
		}
		
	}
	if (XorY + (WidgetSize * (-WidgetSizeAlignment + 1)).X > LoadSize.X || XorY + (WidgetSize * (-WidgetSizeAlignment + 1)).X < 0)
	{
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Top)
		{
			return FVector2D(XorY, (LoadSize + WidgetSize).Y / 2);
		}
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Bottom)
		{
			return FVector2D(XorY, (LoadSize - WidgetSize).Y / 2);
		}
	}
	*/
	float XorYqu, UnknownQua, Unknown;

	// 焦点在X轴上
	if (LoadSize.X > LoadSize.Y)
	{
		// 判定椭圆长轴
		float a = LoadSize.X / 2.0 ;
		// 判定椭圆短轴
		float b = LoadSize.Y / 2.0 ;

		// 长轴平方
		float Aqua = FMath::Square(a);

		// 短轴平方
		float Bqua = FMath::Square(b);

		switch (SpherePosAlignment)
		{
		case ESpherePosAlignment::SAlign_Top:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square(XorY - (LoadSize.X / 2));
			// Y平方
			UnknownQua = (1 - (XorYqu / Aqua)) * Bqua;
			// 求出Y值，因为我们取椭圆上轴的值，所以为正
			Unknown = FMath::Sqrt(UnknownQua);
			return FVector2D(XorY, LoadSize.Y / 2 - Unknown);
			break;
		case ESpherePosAlignment::SAlign_Left:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((LoadSize.Y / 2 - XorY));
			// X平方
			UnknownQua = (1 - (XorYqu / Bqua)) * Aqua;
			// 求出Y值，因为我们取椭圆左轴的值，所以为负
			Unknown = - FMath::Sqrt(UnknownQua);
			return FVector2D(Unknown + (LoadSize.X / 2) , XorY);
			break;
		case ESpherePosAlignment::SAlign_Bottom:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((XorY - (LoadSize.X / 2)));
			// Y平方
			UnknownQua = (1 - (XorYqu / Aqua)) * Bqua;
			// 求出Y值，因为我们取椭圆下轴的值，所以为负
			Unknown = -FMath::Sqrt(UnknownQua);
			return FVector2D(XorY, LoadSize.Y / 2 - Unknown);
			break;
		case ESpherePosAlignment::SAlign_Right:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((LoadSize.Y / 2 - XorY));
			// X平方
			UnknownQua = (1 - (XorYqu / Bqua)) * Aqua;
			// 求出Y值，因为我们取椭圆右轴的值，所以为正
			Unknown = FMath::Sqrt(UnknownQua);
			return FVector2D(Unknown + (LoadSize.X / 2), XorY);
			break;
		}
	}
	// 焦点在Y轴上
	else
	{
		// 判定椭圆长轴
		float a = LoadSize.Y / 2.0;
		// 判定椭圆短轴
		float b = LoadSize.X / 2.0;

		// 长轴平方
		float Aqua = FMath::Square(a);

		// 短轴平方
		float Bqua = FMath::Square(b);

		switch (SpherePosAlignment)
		{
		case ESpherePosAlignment::SAlign_Top:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((XorY - (LoadSize.X / 2)));
			// Y平方
			UnknownQua = (1 - (XorYqu / Bqua)) * Aqua;
			// 求出Y值，因为我们取椭圆上轴的值，所以为正
			Unknown = FMath::Sqrt(UnknownQua);
			return FVector2D(XorY, LoadSize.Y / 2 - Unknown);
			break;
		case ESpherePosAlignment::SAlign_Left:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((LoadSize.Y / 2 - XorY));
			// X平方
			UnknownQua = (1 - (XorYqu / Aqua)) * Bqua;
			// 求出Y值，因为我们取椭圆左轴的值，所以为负
			Unknown = -FMath::Sqrt(UnknownQua);
			return FVector2D(Unknown + (LoadSize.X / 2), XorY);
			break;
		case ESpherePosAlignment::SAlign_Bottom:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((XorY - (LoadSize.X / 2)));
			// Y平方
			UnknownQua = (1 - (XorYqu / Bqua)) * Aqua;
			// 求出Y值，因为我们取椭圆上轴的值，所以为负
			Unknown = -FMath::Sqrt(UnknownQua);
			return FVector2D(XorY, LoadSize.Y / 2 - Unknown);
			break;
		case ESpherePosAlignment::SAlign_Right:
			// 自身的X坐标或者Y坐标的平方
			XorYqu = FMath::Square((LoadSize.Y / 2 - XorY));
			// X平方
			UnknownQua = (1 - (XorYqu / Aqua)) * Bqua;
			// 求出Y值，因为我们取椭圆左轴的值，所以为正
			Unknown = FMath::Sqrt(UnknownQua);
			return FVector2D(Unknown + (LoadSize.X / 2), XorY);
			break;
		}
	}
	return FVector2D(0, 0);
}

template<ESpherePosAlignment SpherePosAlignment>
FVector2D GetSpherePointPos(float XorY, float SphereRadius, FVector2D LoadSize, float Interval)
{
	if (XorY > SphereRadius || XorY < -SphereRadius)
	{
		switch (SpherePosAlignment)
		{
		case ESpherePosAlignment::SAlign_Top:
			return FVector2D(XorY, 0);
			break;
		case ESpherePosAlignment::SAlign_Left:
			return FVector2D(0, XorY);
			break;
		case ESpherePosAlignment::SAlign_Bottom:
			return FVector2D(XorY, 0);
			break;
		case ESpherePosAlignment::SAlign_Right:
			return FVector2D(0, XorY);
			break;
		}
	}
	float XorYqu, UnknownQua, Unknown;
	XorYqu = FMath::Square(XorY);
	UnknownQua = FMath::Square(SphereRadius) - XorYqu;
	Unknown = FMath::Sqrt(UnknownQua);
	switch (SpherePosAlignment)
	{
	case ESpherePosAlignment::SAlign_Top:
		return FVector2D(XorY, Unknown);
		break;
	case ESpherePosAlignment::SAlign_Left:
		return FVector2D(-Unknown, XorY);
		break;
	case ESpherePosAlignment::SAlign_Bottom:
		return FVector2D(XorY, -Unknown);
		break;
	case ESpherePosAlignment::SAlign_Right:
		return FVector2D(Unknown, XorY);
		break;
	}

	return FVector2D::ZeroVector;
}

SSphereCurBox::SSphereCurBox()
{
	bClippingProxy = true;
}

SSphereCurBox::FSlot& SSphereCurBox::Slot()
{
	return *(new SSphereCurBox::FSlot());
}

void SSphereCurBox::Construct(const FArguments& InArgs)
{
	check(InArgs._Style);

	Style = InArgs._Style;
	ScrollBarStyle = InArgs._ScrollBarStyle;
	bIsScrolling = false;
	bAnimateScroll = false;
	bAllowsRightClickDragScrolling = true;
	AmountScrolledWhileRightMouseDown = 0;
	PendingScrollTriggerAmount = 0;
	bShowSoftwareCursor = false;
	SoftwareCursorPosition = FVector2D::ZeroVector;
	ShadowFadeDistance = InArgs._ShadowFadeDistance;
	SpherePosAlignment = InArgs._SpherePosAlignment;
	WidgetIntervalPos = InArgs._WidgetIntervalPos;
	SphereRadius = InArgs._SphereRadius;
	bScrollToEnd = false;
	bIsScrollingActiveTimerRegistered = false;
	ConsumeMouseWheel = InArgs._ConsumeMouseWheel;
	TickScrollDelta = 0;
	bAlignedWidget = InArgs._bAlignedWidget;
	AllowOverscroll = InArgs._AllowOverscroll;
	OnUserScrolled = InArgs._OnUserScrolled;
	OnAlignedScrolled = InArgs._OnAlignedScrolled;
	OnCenterAlignedScrolled = InArgs._OnCenterAlignedScrolled;
	bLoopChildren = InArgs._bLoopChildren;
	bTouchPanningCapture = false;
	ShadowSpeed = 15.f;
	MonuseScrollKey = InArgs._MonuseScrollKey;

	if (InArgs._ExternalScrollbar.IsValid())
	{
		// An external scroll bar was specified by the user
		ScrollBar = InArgs._ExternalScrollbar;
		ScrollBar->SetOnUserScrolled(FOnUserScrolled::CreateSP(this, &SSphereCurBox::SphereCurBox_OnUserScrolled));
		bScrollBarIsExternal = true;
	}
	else
	{
		// Make a scroll bar 
		ScrollBar = ConstructScrollBar();
		ScrollBar->SetDragFocusCause(InArgs._ScrollBarDragFocusCause);
		ScrollBar->SetThickness(InArgs._ScrollBarThickness);
		ScrollBar->SetUserVisibility(InArgs._ScrollBarVisibility);
		ScrollBar->SetScrollBarAlwaysVisible(InArgs._ScrollBarAlwaysVisible);

		bScrollBarIsExternal = false;
	}

	SAssignNew(SphereCurPanel, SSphereCurPanel, InArgs.Slots)
		.Clipping(InArgs._Clipping)
		.SpherePosAlignment(InArgs._SpherePosAlignment)
		.WidgetIntervalPos(InArgs._WidgetIntervalPos)
		.SphereRadius(InArgs._SphereRadius);

	if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
	{
		ConstructVerticalLayout();
	}
	else
	{
		ConstructHorizontalLayout();
	}

	ScrollBar->SetState(0.0f, 1.0f);
}

TSharedPtr<SScrollBar> SSphereCurBox::ConstructScrollBar()
{
	return TSharedPtr<SScrollBar>(SNew(SScrollBar)
		.Style(ScrollBarStyle)
		.Orientation(SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right ? Orient_Vertical : Orient_Horizontal)
		.OnUserScrolled(this, &SSphereCurBox::SphereCurBox_OnUserScrolled));
}

void SSphereCurBox::ConstructVerticalLayout()
{
	TSharedPtr<SOverlay> PanelAndScrollbar;
	this->ChildSlot
		[
			SAssignNew(PanelAndScrollbar, SOverlay)

			+ SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.Padding(FMargin(0.0f, 0.0f, 0.0f, 1.0f))
		[
			// Scroll panel that presents the scrolled content
			SphereCurPanel.ToSharedRef()
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Top)
		[
			// Shadow: Hint to scroll up
			SNew(SImage)
			.Visibility(EVisibility::HitTestInvisible)
		.ColorAndOpacity(this, &SSphereCurBox::GetStartShadowOpacity)
		.Image(&Style->TopShadowBrush)
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Bottom)
		[
			// Shadow: a hint to scroll down
			SNew(SImage)
			.Visibility(EVisibility::HitTestInvisible)
		.ColorAndOpacity(this, &SSphereCurBox::GetEndShadowOpacity)
		.Image(&Style->BottomShadowBrush)
		]
		]
		];

	if (!bScrollBarIsExternal)
	{
		PanelAndScrollbar->AddSlot()
			.HAlign(SpherePosAlignment == ESpherePosAlignment::SAlign_Left ? HAlign_Right : HAlign_Left)
			.VAlign(EVerticalAlignment::VAlign_Fill)
			[
				ScrollBar.ToSharedRef()
			];
	}
}

void SSphereCurBox::ConstructHorizontalLayout()
{
	TSharedPtr<SOverlay> PanelAndScrollbar;
	this->ChildSlot
		[
			SAssignNew(PanelAndScrollbar, SOverlay)

			+ SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Fill)
		.VAlign(EVerticalAlignment::VAlign_Fill)
		[
			SNew(SOverlay)

			+ SOverlay::Slot()
		.Padding(FMargin(0.0f, 0.0f, 1.0f, 0.0f))
		[
			// Scroll panel that presents the scrolled content
			SphereCurPanel.ToSharedRef()
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		[
			// Shadow: Hint to left
			SNew(SImage)
			.Visibility(EVisibility::HitTestInvisible)
		.ColorAndOpacity(this, &SSphereCurBox::GetStartShadowOpacity)
		.Image(&Style->LeftShadowBrush)
		]

	+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		[
			// Shadow: a hint to scroll right
			SNew(SImage)
			.Visibility(EVisibility::HitTestInvisible)
		.ColorAndOpacity(this, &SSphereCurBox::GetEndShadowOpacity)
		.Image(&Style->RightShadowBrush)
		]
		]
		];

	if (!bScrollBarIsExternal)
	{
		PanelAndScrollbar->AddSlot()
			.HAlign(EHorizontalAlignment::HAlign_Fill)
			.VAlign(SpherePosAlignment == ESpherePosAlignment::SAlign_Top ? VAlign_Bottom : VAlign_Top)
			[
				ScrollBar.ToSharedRef()
			];
	}
}

SSphereCurBox::FSlot& SSphereCurBox::AddSlot()
{
	SSphereCurBox::FSlot& NewSlot = *new SSphereCurBox::FSlot();
	SphereCurPanel->Children.Add(&NewSlot);

	Invalidate(EInvalidateWidget::Layout);

	return NewSlot;
}

void SSphereCurBox::RemoveSlot(const TSharedRef<SWidget>& WidgetToRemove)
{
	TPanelChildren<SSphereCurBox::FSlot>& Children = SphereCurPanel->Children;
	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		if (Children[SlotIndex].GetWidget() == WidgetToRemove)
		{
			Children.RemoveAt(SlotIndex);
			return;
		}
	}
}

void SSphereCurBox::ClearChildren()
{
	SphereCurPanel->Children.Empty();
}

bool SSphereCurBox::IsRightClickScrolling() const
{
	return FSlateApplication::IsInitialized() && AmountScrolledWhileRightMouseDown >= FSlateApplication::Get().GetDragTriggerDistance();
}

float SSphereCurBox::GetScrollOffset() const
{
	return DesiredScrollOffset;
}

void SSphereCurBox::SetScrollOffset(float NewScrollOffset)
{
	DesiredScrollOffset = NewScrollOffset;
	bScrollToEnd = false;
}

float SSphereCurBox::GetViewFraction() const
{
	const FGeometry ScrollPanelGeometry = FindChildGeometry(CachedGeometry, SphereCurPanel.ToSharedRef());
	const float ContentSize = GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize());

	return FMath::Clamp<float>(GetScrollComponentFromVector(CachedGeometry.GetLocalSize()) > 0 ? GetScrollComponentFromVector(ScrollPanelGeometry.Size) / ContentSize : 1, 0.0f, 1.0f);
}

float SSphereCurBox::GetViewOffsetFraction() const
{
	const FGeometry ScrollPanelGeometry = FindChildGeometry(CachedGeometry, SphereCurPanel.ToSharedRef());
	const float ContentSize = GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize());

	const float ViewFraction = GetViewFraction();
	if (bLoopChildren)
	{
		return DesiredScrollOffset / ContentSize;
	}
	return FMath::Clamp<float>(DesiredScrollOffset / ContentSize, 0.0, 1.0 - ViewFraction);
}

void SSphereCurBox::ScrollDescendantIntoView(const TSharedPtr<SWidget>& WidgetToFind, bool InAnimateScroll, float Padding)
{
	ScrollIntoViewRequest = [this, WidgetToFind, InAnimateScroll, Padding](FGeometry AllottedGeometry) {
		InternalScrollDescendantIntoView(AllottedGeometry, WidgetToFind, InAnimateScroll, Padding);
	};

	BeginInertialScrolling();
}

void SSphereCurBox::ScrollDescendantIntoWidget(const TSharedPtr<SWidget>& WidgetToFind, const TSharedPtr<SWidget>& IntoWidget, bool InAnimateScroll)
{
	ScrollIntoViewRequest = [this, WidgetToFind, IntoWidget, InAnimateScroll](FGeometry AllottedGeometry) {
		InternalScrollDescendantIntoWidget(AllottedGeometry, WidgetToFind, IntoWidget, InAnimateScroll);
	};

	BeginInertialScrolling();
}

void SSphereCurBox::ScrollDescendantIntoOffset(float NewOffset, bool InAnimateScroll)
{
	ScrollIntoViewRequest = [this, NewOffset, InAnimateScroll](FGeometry AllottedGeometry) {
		InternalScrollDescendantIntoOffice(AllottedGeometry, NewOffset, InAnimateScroll);
	};

	BeginInertialScrolling();
}

void SSphereCurBox::ScrollToStart()
{
	SetScrollOffset(0);
}

void SSphereCurBox::ScrollToEnd()
{
	bScrollToEnd = true;

	Invalidate(EInvalidateWidget::Layout);
}


void SSphereCurBox::SetSpherePosAlignment(ESpherePosAlignment InSpherePosAlignment)
{
	if (SpherePosAlignment != InSpherePosAlignment)
	{
		SpherePosAlignment = InSpherePosAlignment;
		if (!bScrollBarIsExternal)
		{
			ScrollBar = ConstructScrollBar();
		}
		SphereCurPanel->SetSpherePosAlignment(InSpherePosAlignment);
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left|| SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
		{
			ConstructVerticalLayout();
		}
		else
		{
			ConstructHorizontalLayout();
		}
	}
}

void SSphereCurBox::SetScrollBarVisibility(EVisibility InVisibility)
{
	ScrollBar->SetUserVisibility(InVisibility);
}

void SSphereCurBox::SetScrollBarThickness(FVector2D InThickness)
{
	ScrollBar->SetThickness(InThickness);
}

void SSphereCurBox::SetScrollBarAlwaysVisible(bool InAlwaysVisible)
{
	ScrollBar->SetScrollBarAlwaysVisible(InAlwaysVisible);
}


void SSphereCurBox::SetChildrenAlignment(FVector2D InChildrenAlignment)
{
	ChildrenAlignment = InChildrenAlignment;
	SphereCurPanel->SetChildrenAlignment(InChildrenAlignment);
}

void SSphereCurBox::SetAllowOverscroll(EAllowOverscroll InAllowOverscroll)
{
	AllowOverscroll = InAllowOverscroll;
}

void SSphereCurBox::Tick(const FGeometry & AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	CachedGeometry = AllottedGeometry;

	if (bTouchPanningCapture && (FSlateApplication::Get().GetCurrentTime() - LastScrollTime) > 0.10)
	{
		InertialScrollManager.ClearScrollVelocity();
	}

	// If we needed a widget to be scrolled into view, make that happen.
	if (ScrollIntoViewRequest)
	{
		ScrollIntoViewRequest(AllottedGeometry);
		ScrollIntoViewRequest = nullptr;
	}

	const FGeometry ScrollPanelGeometry = FindChildGeometry(AllottedGeometry, SphereCurPanel.ToSharedRef());
	const float ContentSize = GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize());

	if (bScrollToEnd && bLoopChildren)
	{
		DesiredScrollOffset = FMath::Max(ContentSize - GetScrollComponentFromVector(ScrollPanelGeometry.GetLocalSize()), 0.0f);
		bScrollToEnd = false;
	}

	// If this scroll box has no size, do not compute a view fraction because it will be wrong and causes pop in when the size is available
	const float ViewFraction = GetViewFraction();
	const float ViewOffset = GetViewOffsetFraction();

	// Update the scrollbar with the clamped version of the offset
	float TargetPhysicalOffset = GetScrollComponentFromVector(ViewOffset*SphereCurPanel->GetDesiredSize());
	if (AllowOverscroll == EAllowOverscroll::Yes)
	{
		TargetPhysicalOffset += Overscroll.GetOverscroll(AllottedGeometry);
	}

	const bool bWasScrolling = bIsScrolling;
	bIsScrolling = !FMath::IsNearlyEqual(TargetPhysicalOffset, SphereCurPanel->PhysicalOffset, 0.001f);
	SphereCurPanel->PhysicalOffset = (bAnimateScroll)
		? FMath::FInterpTo(SphereCurPanel->PhysicalOffset, TargetPhysicalOffset, InDeltaTime, ShadowSpeed)
		: TargetPhysicalOffset;


	if (bWasScrolling && !bIsScrolling)
	{
		Invalidate(EInvalidateWidget::Layout);
	}

	ScrollBar->SetState(ViewOffset, ViewFraction);
	//if (!ScrollBar->IsNeeded())
	//{
	//	// We cannot scroll, so ensure that there is no offset.
	//	SphereCurPanel->PhysicalOffset = 0.0f;
	//}

}

bool SSphereCurBox::ComputeVolatility() const
{
	return bIsScrolling || IsRightClickScrolling();
}

FReply SSphereCurBox::OnPreviewMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if (MouseEvent.IsTouchEvent() && !bFingerOwningTouchInteraction.IsSet())
	{
		// Clear any inertia 
		InertialScrollManager.ClearScrollVelocity();
		// We have started a new interaction; track how far the user has moved since they put their finger down.
		AmountScrolledWhileRightMouseDown = 0;
		PendingScrollTriggerAmount = 0;
		// Someone put their finger down in this list, so they probably want to drag the list.
		bFingerOwningTouchInteraction = MouseEvent.GetPointerIndex();

		Invalidate(EInvalidateWidget::Layout);
	}
	return FReply::Unhandled();
}

FReply SSphereCurBox::OnMouseButtonDown(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if (!bFingerOwningTouchInteraction.IsSet())
	{
		EndInertialScrolling();
	}

	if (MouseEvent.IsTouchEvent())
	{
		return FReply::Handled();
	}
	else
	{
		if (MouseEvent.GetEffectingButton() == MonuseScrollKey && bAllowsRightClickDragScrolling)
		{
			AmountScrolledWhileRightMouseDown = 0;

			Invalidate(EInvalidateWidget::Layout);

			return FReply::Handled();
		}
	}

	return FReply::Unhandled();
}

FReply SSphereCurBox::OnMouseButtonUp(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == MonuseScrollKey && bAllowsRightClickDragScrolling)
	{
		if (!bIsScrollingActiveTimerRegistered && IsRightClickScrolling())
		{
			// Register the active timer to handle the inertial scrolling
			CachedGeometry = MyGeometry;
			BeginInertialScrolling();
		}

		AmountScrolledWhileRightMouseDown = 0;

		Invalidate(EInvalidateWidget::Layout);

		FReply Reply = FReply::Handled().ReleaseMouseCapture();
		bShowSoftwareCursor = false;

		// If we have mouse capture, snap the mouse back to the closest location that is within the panel's bounds
		if (HasMouseCapture())
		{
			FSlateRect PanelScreenSpaceRect = MyGeometry.GetLayoutBoundingRect();
			FVector2D CursorPosition = MyGeometry.LocalToAbsolute(SoftwareCursorPosition);

			FIntPoint BestPositionInPanel(
				FMath::RoundToInt(FMath::Clamp(CursorPosition.X, PanelScreenSpaceRect.Left, PanelScreenSpaceRect.Right)),
				FMath::RoundToInt(FMath::Clamp(CursorPosition.Y, PanelScreenSpaceRect.Top, PanelScreenSpaceRect.Bottom))
			);

			Reply.SetMousePos(BestPositionInPanel);
		}

		return Reply;
	}

	return FReply::Unhandled();
}

FReply SSphereCurBox::OnMouseMove(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	const float ScrollByAmountScreen = GetScrollComponentFromVector(MouseEvent.GetCursorDelta());
	const float ScrollByAmountLocal = ScrollByAmountScreen / MyGeometry.Scale;

	if (MouseEvent.IsTouchEvent())
	{
		FReply Reply = FReply::Unhandled();

		if (!bTouchPanningCapture)
		{
			if (bFingerOwningTouchInteraction.IsSet() && MouseEvent.IsTouchEvent() && !HasMouseCapture())
			{
				PendingScrollTriggerAmount += ScrollByAmountScreen;

				if (FMath::Abs(PendingScrollTriggerAmount) > FSlateApplication::Get().GetDragTriggerDistance())
				{
					bTouchPanningCapture = true;
					ScrollBar->BeginScrolling();

					Reply = FReply::Handled().CaptureMouse(AsShared());
				}
				else
				{
					Reply = FReply::Handled();
				}
			}
		}
		else
		{
			if (bFingerOwningTouchInteraction.IsSet() && HasMouseCaptureByUser(MouseEvent.GetUserIndex(), MouseEvent.GetPointerIndex()))
			{
				LastScrollTime = FSlateApplication::Get().GetCurrentTime();
				InertialScrollManager.AddScrollSample(-ScrollByAmountScreen, FSlateApplication::Get().GetCurrentTime());
				ScrollBy(MyGeometry, -ScrollByAmountLocal, EAllowOverscroll::Yes, false);

				Reply = FReply::Handled();
			}
			
		}

		return Reply;
	}
	else
	{
		if (MouseEvent.IsMouseButtonDown(MonuseScrollKey) && bAllowsRightClickDragScrolling)
		{
			// If scrolling with the right mouse button, we need to remember how much we scrolled.
			// If we did not scroll at all, we will bring up the context menu when the mouse is released.
			AmountScrolledWhileRightMouseDown += FMath::Abs(ScrollByAmountScreen);

			// Has the mouse moved far enough with the right mouse button held down to start capturing
			// the mouse and dragging the view?
			if (IsRightClickScrolling())
			{
				InertialScrollManager.AddScrollSample(-ScrollByAmountScreen, FPlatformTime::Seconds());
				//UE_LOG(LogTemp, Log, TEXT("Spped:%f"), -ScrollByAmountScreen);
				const bool bDidScroll = ScrollBy(MyGeometry, -ScrollByAmountLocal, AllowOverscroll, false);

				FReply Reply = FReply::Handled();

				// Capture the mouse if we need to
				if (HasMouseCapture() == false)
				{
					Reply.CaptureMouse(AsShared()).UseHighPrecisionMouseMovement(AsShared());
					SoftwareCursorPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
					bShowSoftwareCursor = true;
				}

				// Check if the mouse has moved.
				if (bDidScroll)
				{
					SetScrollComponentOnVector(SoftwareCursorPosition, GetScrollComponentFromVector(SoftwareCursorPosition) + GetScrollComponentFromVector(MouseEvent.GetCursorDelta()));
				}

				return Reply;
			}
		}
	}

	return FReply::Unhandled();
}

void SSphereCurBox::OnMouseEnter(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if (MouseEvent.IsTouchEvent())
	{
		if (!bFingerOwningTouchInteraction.IsSet())
		{
			// If we currently do not have touch capture, allow this widget to begin scrolling on pointer enter events
			// if it comes from a child widget
			if (MyGeometry.IsUnderLocation(MouseEvent.GetLastScreenSpacePosition()))
			{
				bFingerOwningTouchInteraction = MouseEvent.GetPointerIndex();
			}
		}
	}
}

void SSphereCurBox::OnMouseLeave(const FPointerEvent & MouseEvent)
{
	if (HasMouseCapture() == false)
	{
		// No longer scrolling (unless we have mouse capture)
		if (AmountScrolledWhileRightMouseDown != 0)
		{
			AmountScrolledWhileRightMouseDown = 0;
			Invalidate(EInvalidateWidget::Layout);
		}

		if (MouseEvent.IsTouchEvent())
		{
			bFingerOwningTouchInteraction.Reset();
		}
	}
}

FReply SSphereCurBox::OnMouseWheel(const FGeometry & MyGeometry, const FPointerEvent & MouseEvent)
{
	if ((ConsumeMouseWheel != EConsumeMouseWheel::Never) || ConsumeMouseWheel == EConsumeMouseWheel::Always)
	{
		// Make sure scroll velocity is cleared so it doesn't fight with the mouse wheel input
		InertialScrollManager.ClearScrollVelocity();

		const bool bScrollWasHandled = ScrollBy(MyGeometry, -MouseEvent.GetWheelDelta() * GetGlobalScrollAmount(), EAllowOverscroll::No, false);

		if (bScrollWasHandled && !bIsScrollingActiveTimerRegistered)
		{
			// Register the active timer to handle the inertial scrolling
			CachedGeometry = MyGeometry;
			BeginInertialScrolling();
		}

		return bScrollWasHandled ? FReply::Handled() : FReply::Unhandled();
	}
	else
	{
		return FReply::Unhandled();
	}
}

FCursorReply SSphereCurBox::OnCursorQuery(const FGeometry & MyGeometry, const FPointerEvent & CursorEvent) const
{
	if (IsRightClickScrolling())
	{
		// We hide the native cursor as we'll be drawing the software EMouseCursor::GrabHandClosed cursor
		return FCursorReply::Cursor(EMouseCursor::None);
	}
	else
	{
		return FCursorReply::Unhandled();
	}
}

int32 SSphereCurBox::OnPaint(const FPaintArgs & Args, const FGeometry & AllottedGeometry, const FSlateRect & MyCullingRect, FSlateWindowElementList & OutDrawElements, int32 LayerId, const FWidgetStyle & InWidgetStyle, bool bParentEnabled) const
{
	int32 NewLayerId = SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	if (!bShowSoftwareCursor)
	{
		return NewLayerId;
	}

	const FSlateBrush* Brush = FCoreStyle::Get().GetBrush(TEXT("SoftwareCursor_Grab"));

	FSlateDrawElement::MakeBox(
		OutDrawElements,
		++NewLayerId,
		AllottedGeometry.ToPaintGeometry(SoftwareCursorPosition - (Brush->ImageSize / 2), Brush->ImageSize),
		Brush
	);

	return NewLayerId;
}

FReply SSphereCurBox::OnTouchEnded(const FGeometry & MyGeometry, const FPointerEvent & InTouchEvent)
{
	CachedGeometry = MyGeometry;

	if (HasMouseCaptureByUser(InTouchEvent.GetUserIndex(), InTouchEvent.GetPointerIndex()))
	{
		AmountScrolledWhileRightMouseDown = 0;
		PendingScrollTriggerAmount = 0;
		bFingerOwningTouchInteraction.Reset();
		bTouchPanningCapture = false;

		ScrollBar->EndScrolling();

		Invalidate(EInvalidateWidget::Layout);

		BeginInertialScrolling();

		return FReply::Handled().ReleaseMouseCapture();
	}

	return FReply::Unhandled();
}

void SSphereCurBox::OnMouseCaptureLost(const FCaptureLostEvent & CaptureLostEvent)
{
	SCompoundWidget::OnMouseCaptureLost(CaptureLostEvent);
}

FNavigationReply SSphereCurBox::OnNavigation(const FGeometry & MyGeometry, const FNavigationEvent & InNavigationEvent)
{
	TSharedPtr<SWidget> FocusedChild;
	int32 FocusedChildIndex = -1;
	int32 FocusedChildDirection = 0;

	// Find the child with focus currently so that we can find the next logical child we're going to move to.
	TPanelChildren<SSphereCurBox::FSlot>& Children = SphereCurPanel->Children;
	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		if (Children[SlotIndex].GetWidget()->HasUserFocus(InNavigationEvent.GetUserIndex()).IsSet() ||
			Children[SlotIndex].GetWidget()->HasUserFocusedDescendants(InNavigationEvent.GetUserIndex()))
		{
			FocusedChild = Children[SlotIndex].GetWidget();
			FocusedChildIndex = SlotIndex;
			break;
		}
	}

	if (FocusedChild.IsValid())
	{
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
		{
			switch (InNavigationEvent.GetNavigationType())
			{
			case EUINavigation::Up:
				FocusedChildDirection = -1;
				break;
			case EUINavigation::Down:
				FocusedChildDirection = 1;
				break;
			default:
				// If we don't handle this direction in our current orientation we can 
				// just allow the behavior of the boundary rule take over.
				return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
			}
		}
		else
		{
			switch (InNavigationEvent.GetNavigationType())
			{
			case EUINavigation::Left:
				FocusedChildDirection = -1;
				break;
			case EUINavigation::Right:
				FocusedChildDirection = 1;
				break;
			default:
				// If we don't handle this direction in our current orientation we can 
				// just allow the behavior of the boundary rule take over.
				return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
			}
		}

		// If the focused child index is in a valid range we know we can successfully focus
		// the new child we're moving focus to.
		if (FocusedChildDirection != 0)
		{
			TSharedPtr<SWidget> NextFocusableChild;

			// Search in the direction we need to move for the next focusable child of the scrollbox.
			for (int32 ChildIndex = FocusedChildIndex + FocusedChildDirection; ChildIndex >= 0 && ChildIndex < Children.Num(); ChildIndex += FocusedChildDirection)
			{
				TSharedPtr<SWidget> PossiblyFocusableChild = GetKeyboardFocusableWidget(Children[ChildIndex].GetWidget());
				if (PossiblyFocusableChild.IsValid())
				{
					NextFocusableChild = PossiblyFocusableChild;
					break;
				}
			}

			// If we found a focusable child, scroll to it, and shift focus.
			if (NextFocusableChild.IsValid())
			{
				InternalScrollDescendantIntoView(MyGeometry, NextFocusableChild, false, 0.f);
				return FNavigationReply::Explicit(NextFocusableChild);
			}
		}
	}

	return SCompoundWidget::OnNavigation(MyGeometry, InNavigationEvent);
}


void SSphereCurBox::SphereCurBox_OnUserScrolled(float InScrollOffsetFraction)
{
	const float ContentSize = GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize());
	const FGeometry ScrollPanelGeometry = FindChildGeometry(CachedGeometry, SphereCurPanel.ToSharedRef());

	// Clamp to max scroll offset
	DesiredScrollOffset = FMath::Min(InScrollOffsetFraction * ContentSize, ContentSize - GetScrollComponentFromVector(ScrollPanelGeometry.GetLocalSize()));
	OnUserScrolled.ExecuteIfBound(DesiredScrollOffset);

	Invalidate(EInvalidateWidget::Layout);
}


FSlateColor SSphereCurBox::GetStartShadowOpacity() const
{
	const float ShadowOpacity = FMath::Clamp(SphereCurPanel->PhysicalOffset / ShadowFadeDistance, 0.0f, 1.0f);

	return FLinearColor(1.0f, 1.0f, 1.0f, ShadowOpacity);
}

FSlateColor SSphereCurBox::GetEndShadowOpacity() const
{
	const float ShadowOpacity = (ScrollBar->DistanceFromBottom() * GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize()) / ShadowFadeDistance);

	return FLinearColor(1.0f, 1.0f, 1.0f, ShadowOpacity);
}

bool SSphereCurBox::CanUseInertialScroll(float ScrollAmount) const
{
	const auto CurrentOverscroll = Overscroll.GetOverscroll(CachedGeometry);

	return CurrentOverscroll == 0.f || FMath::Sign(CurrentOverscroll) != FMath::Sign(ScrollAmount);
}

void SSphereCurBox::InertialOffsetOver(bool IsForwardMovement)
{
	bAnimateScroll = true;
	float WidgetCenter = GetScrollComponentFromVector(this->GetCachedGeometry().GetLocalSize()) / 2;
	float MinDistance = WidgetIntervalPos;
	float Dir = 0.f;
	int MinDistanceWidgetID = 0;
	for (int i = 0; i < SphereCurPanel->Children.Num(); ++i)
	{
		SSphereCurBox::FSlot &Item = SphereCurPanel->Children[i];
		FVector2D PointPos = GetChildWidgetSpherePointPos(Item);
		float DistanceDir = FMath::Abs(WidgetCenter - GetScrollComponentFromVector(PointPos));

		if (DistanceDir < MinDistance)
		{
			/*
			float PointDir = GetScrollComponentFromVector(PointPos) - WidgetCenter;
			if (PointDir > 0 && IsForwardMovement)
			{
				MinDistance = DistanceDir;
				Dir = GetScrollComponentFromVector(PointPos) - WidgetCenter;
				MinDistanceWidgetID = i;
			}
			else if (PointDir < 0 && !IsForwardMovement)
			{
				MinDistance = DistanceDir;
				Dir = GetScrollComponentFromVector(PointPos) - WidgetCenter;
				MinDistanceWidgetID = i;
			}*/
			MinDistance = DistanceDir;
			Dir = GetScrollComponentFromVector(PointPos) - WidgetCenter;
			MinDistanceWidgetID = i;
		}
	}
	ShadowSpeed = 2.f;
	DesiredScrollOffset += Dir;
	OnAlignedScrolled.ExecuteIfBound(SphereCurPanel->Children[MinDistanceWidgetID].GetWidget());
}

void SSphereCurBox::FindChildCenterWidget()
{
	float WidgetCenter = GetScrollComponentFromVector(this->GetCachedGeometry().GetLocalSize()) / 2;
	float MinDistance = WidgetIntervalPos;
	for (int i = 0; i < SphereCurPanel->Children.Num(); ++i)
	{
		SSphereCurBox::FSlot &Item = SphereCurPanel->Children[i];
		FVector2D PointPos = GetChildWidgetSpherePointPos(Item);
		float DistanceDir = FMath::Abs(WidgetCenter - GetScrollComponentFromVector(PointPos));

		if (DistanceDir < MinDistance)
		{
			OnCenterAlignedScrolled.ExecuteIfBound(Item.GetWidget());
			break;
		}
	}
}

FVector2D SSphereCurBox::GetChildWidgetSpherePointPos(const SSphereCurBox::FSlot& Child)
{
	const FMargin WidgetPadding = Child.SlotPadding.Get();
	const FVector2D WidgetSizeAlignment = Child.GetWidget()->GetDesiredSize() * ChildrenAlignment;
	const FVector2D WidgetPos = Child.GetWidget()->GetCachedGeometry().Position;
	
	float PointX = 0.f, PointY = 0.f;
	switch (SpherePosAlignment)
	{
	case ESpherePosAlignment::SAlign_Top:
		PointX = WidgetPos.X - WidgetPadding.Left + WidgetSizeAlignment.X;
		PointY = WidgetPos.Y - WidgetPadding.Top + WidgetSizeAlignment.Y;
		break;
	case ESpherePosAlignment::SAlign_Left:
		PointX = WidgetPos.X - WidgetPadding.Left + WidgetSizeAlignment.X;
		PointY = WidgetPos.Y - WidgetPadding.Top + WidgetSizeAlignment.Y;
		break;
	case ESpherePosAlignment::SAlign_Bottom:
		PointX = WidgetPos.X + WidgetPadding.Right + WidgetSizeAlignment.X;
		PointY = WidgetPos.Y + WidgetPadding.Bottom + WidgetSizeAlignment.Y;
		break;
	case ESpherePosAlignment::SAlign_Right:
		PointX = WidgetPos.X + WidgetPadding.Right + WidgetSizeAlignment.X;
		PointY = WidgetPos.Y + WidgetPadding.Bottom + WidgetSizeAlignment.Y;
		break;
	}
	return FVector2D(PointX, PointY);
}

int SSphereCurBox::GetPanelChildIndex(const TSharedPtr<SWidget>& WidgetToFind)
{
	int ChildrenID = -1;
	for (int i = 0; i < SphereCurPanel->Children.Num(); ++i)
	{
		SSphereCurBox::FSlot &Item = SphereCurPanel->Children[i];
		if (Item.GetWidget() == WidgetToFind)
		{
			ChildrenID = i;
			break;
		}
	}
	return ChildrenID;
}

void SSphereCurBox::BeginInertialScrolling()
{
	if (!UpdateInertialScrollHandle.IsValid())
	{
		bIsScrolling = true;
		bIsScrollingActiveTimerRegistered = true;
		UpdateInertialScrollHandle = RegisterActiveTimer(0.f, FWidgetActiveTimerDelegate::CreateSP(this, &SSphereCurBox::UpdateInertialScroll));
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

void SSphereCurBox::EndInertialScrolling()
{
	bIsScrolling = false;
	bIsScrollingActiveTimerRegistered = false;
	if (UpdateInertialScrollHandle.IsValid())
	{
		UnRegisterActiveTimer(UpdateInertialScrollHandle.ToSharedRef());
		UpdateInertialScrollHandle.Reset();
	}

	InertialScrollManager.ClearScrollVelocity();
}

TSharedPtr<SWidget> SSphereCurBox::GetKeyboardFocusableWidget(TSharedPtr<SWidget> InWidget)
{
	if (InWidget->SupportsKeyboardFocus())
	{
		return InWidget;
	}
	else
	{
		FChildren* Children = InWidget->GetChildren();
		for (int32 i = 0; i < Children->Num(); ++i)
		{
			TSharedPtr<SWidget> ChildWidget = Children->GetChildAt(i);
			TSharedPtr<SWidget> FoucusableWidget = GetKeyboardFocusableWidget(ChildWidget);
			if (FoucusableWidget.IsValid())
			{
				return FoucusableWidget;
			}
		}
	}
	return nullptr;
}

bool SSphereCurBox::InternalScrollDescendantIntoView(const FGeometry & MyGeometry, const TSharedPtr<SWidget>& WidgetToFind, bool InAnimateScroll, float Padding)
{
	TSet< TSharedRef<SWidget> > WidgetsToFind;
	{
		if (WidgetToFind.IsValid())
		{
			WidgetsToFind.Add(WidgetToFind.ToSharedRef());
		}
	}
	TMap<TSharedRef<SWidget>, FArrangedWidget> Result;

	FindChildGeometries(MyGeometry, WidgetsToFind, Result);

	if (WidgetToFind.IsValid())
	{
		FArrangedWidget* WidgetGeometry = Result.Find(WidgetToFind.ToSharedRef());
		if (!WidgetGeometry)
		{
			UE_LOG(LogSlate, Warning, TEXT("无法滚动到后代，因为它不是滚动框的子级"));
		}
		else
		{
			// 目标控件相对于父控件的位置
			const float WidgetPosition = GetScrollComponentFromVector(MyGeometry.AbsoluteToLocal(WidgetGeometry->Geometry.GetAbsolutePosition()));
			const float MyPosition = Padding;

			float ScrollOffset = WidgetPosition - MyPosition;

			if (ScrollOffset != 0.0f)
			{
				DesiredScrollOffset = SphereCurPanel->PhysicalOffset;
				ScrollBy(MyGeometry, ScrollOffset/2, EAllowOverscroll::Yes, InAnimateScroll);
			}

			return true;
		}
	}
	return false;
}

bool SSphereCurBox::InternalScrollDescendantIntoWidget(const FGeometry & MyGeometry, const TSharedPtr<SWidget>& WidgetToFind, const TSharedPtr<SWidget>& IntoWidget, bool InAnimateScroll)
{
	TSet< TSharedRef<SWidget> > WidgetsToFind;
	{
		if (WidgetToFind.IsValid() && IntoWidget.IsValid())
		{
			WidgetsToFind.Add(WidgetToFind.ToSharedRef());
			WidgetsToFind.Add(IntoWidget.ToSharedRef());
		}
	}
	TMap<TSharedRef<SWidget>, FArrangedWidget> Result;

	FindChildGeometries(MyGeometry, WidgetsToFind, Result);

	if (WidgetToFind.IsValid() && IntoWidget.IsValid())
	{
		FArrangedWidget* WidgetGeometry = Result.Find(WidgetToFind.ToSharedRef());
		FArrangedWidget* IntoWidgetGeometry = Result.Find(IntoWidget.ToSharedRef());
		if (!WidgetGeometry || !IntoWidgetGeometry)
		{
			UE_LOG(LogSlate, Warning, TEXT("无法滚动到后代，因为他们中有不是滚动框的子级"));
		}
		else
		{
			// 目标控件相对于父控件的位置
			//const float WidgetPosition = GetScrollComponentFromVector(MyGeometry.AbsoluteToLocal(WidgetGeometry->Geometry.GetAbsolutePosition()));
			int WidgetToFindID = GetPanelChildIndex(WidgetToFind);
			// 跳转的控件相对于父控件的位置
			//const float IntoWidgetPosion = GetScrollComponentFromVector(MyGeometry.AbsoluteToLocal(IntoWidgetGeometry->Geometry.GetAbsolutePosition()));
			int IntoWidgetID = GetPanelChildIndex(IntoWidget);

			//float ScrollOffsetDir = WidgetPosition - IntoWidgetPosion;

			float ScrollOffsetDir = (WidgetToFindID - IntoWidgetID) * WidgetIntervalPos;

			if (ScrollOffsetDir != 0.0f)
			{
				ScrollBy(MyGeometry, ScrollOffsetDir, EAllowOverscroll::Yes, InAnimateScroll);
			}

			return true;
		}
	}
	return false;
}

bool SSphereCurBox::InternalScrollDescendantIntoOffice(const FGeometry & MyGeometry, float NewOffset, bool InAnimateScroll)
{
	if (NewOffset != 0.f)
	{
		ScrollBy(MyGeometry, NewOffset, EAllowOverscroll::Yes, InAnimateScroll);
		return true;
	}
	return false;
}

bool SSphereCurBox::ScrollBy(const FGeometry & AllottedGeometry, float LocalScrollAmount, EAllowOverscroll InOverscroll, bool InAnimateScroll)
{
	Invalidate(EInvalidateWidget::LayoutAndVolatility);


	const float ContentSize = GetScrollComponentFromVector(SphereCurPanel->GetDesiredSize());
	const FGeometry ScrollPanelGeometry = FindChildGeometry(AllottedGeometry, SphereCurPanel.ToSharedRef());

	const float PreviousScrollOffset = DesiredScrollOffset;
	if (LocalScrollAmount != 0)
	{
		ShadowSpeed = 15.f;
		const float ScrollMin = 0.0f;
		const float ScrollMax = FMath::Max(ContentSize - GetScrollComponentFromVector(ScrollPanelGeometry.GetLocalSize()), 0.0f);
		//const float ScrollMax = FMath::Max(ContentSize - GetScrollComponentFromVector(ScrollPanelGeometry.GetLocalSize()), 0.0f);

		FindChildCenterWidget();

		if (AllowOverscroll == EAllowOverscroll::Yes && InOverscroll == EAllowOverscroll::Yes && Overscroll.ShouldApplyOverscroll(DesiredScrollOffset == 0, DesiredScrollOffset == ScrollMax, LocalScrollAmount))
		{
			bAnimateScroll = InAnimateScroll;
			Overscroll.ScrollBy(AllottedGeometry, LocalScrollAmount);
		}

		if (bLoopChildren)
		{
			//float PaddingDir = 0;
			//SphereCurPanel->PhysicalOffset = 0;
			// 如果DesiredScrollOffset的值趋近间隔，则设置到该间隔
			if (FMath::Abs(FMath::Abs(DesiredScrollOffset) - WidgetIntervalPos) < 0.01)
			{
				DesiredScrollOffset = FMath::RoundToInt(DesiredScrollOffset / WidgetIntervalPos) * WidgetIntervalPos;
			}

			// 子控件整体大小是否小于控件大小
			if (ContentSize <= GetScrollComponentFromVector(ScrollPanelGeometry.GetLocalSize()))
			{
				//LocalScrollAmount = LocalScrollAmount / 2;
				bAnimateScroll = false;
				// 判定滚动方向
				if (LocalScrollAmount > 0)
				{
					int MaxChilrenIndex = SphereCurPanel->Children.Num();

					/*int WidgetInsertIndex = bAlignedWidget 
						? FMath::CeilToInt((DesiredScrollOffset + LocalScrollAmount) / WidgetIntervalPos)
						: FMath::TruncToInt((DesiredScrollOffset + LocalScrollAmount) / WidgetIntervalPos);*/

					int WidgetInsertIndex = FMath::TruncToInt((DesiredScrollOffset + LocalScrollAmount) / WidgetIntervalPos);
					
					if (WidgetInsertIndex > 0)
					{
						for (int i = 0; i < WidgetInsertIndex; ++i)
						{
							DesiredScrollOffset -= WidgetIntervalPos;
							SphereCurPanel->Children.Move(0, MaxChilrenIndex - 1);
						}
					}
					
					WidgetInsertIndex = FMath::Max(0, WidgetInsertIndex);

					SphereCurPanel->PhysicalOffset = DesiredScrollOffset;

				}
				else
				{
					int MaxChilrenIndex = SphereCurPanel->Children.Num();
					FVector2D PanelSize = ScrollPanelGeometry.GetLocalSize();
					/*int WidgetInsertIndex = bAlignedWidget 
						? FMath::CeilToInt((DesiredScrollOffset - LocalScrollAmount + GetScrollComponentFromVector(PanelSize) - ContentSize) / WidgetIntervalPos)
						: FMath::TruncToInt((DesiredScrollOffset - LocalScrollAmount + GetScrollComponentFromVector(PanelSize) - ContentSize) / WidgetIntervalPos);*/
					int WidgetInsertIndex = FMath::TruncToInt((DesiredScrollOffset + LocalScrollAmount + GetScrollComponentFromVector(PanelSize) - ContentSize) / WidgetIntervalPos);
					if (WidgetInsertIndex < 0)
					{
						for (int i = 0; i < - WidgetInsertIndex; ++i)
						{
							DesiredScrollOffset += WidgetIntervalPos;

							SphereCurPanel->Children.Move(MaxChilrenIndex - 1, 0);
						}
					}
					WidgetInsertIndex = FMath::Min(0, WidgetInsertIndex);

					SphereCurPanel->PhysicalOffset = DesiredScrollOffset;

				}

			}
			else if (DesiredScrollOffset + LocalScrollAmount > ScrollMax)
				{
					bAnimateScroll = false;
					float SizeDir = DesiredScrollOffset + LocalScrollAmount - ScrollMax;
					int MaxChilrenIndex = SphereCurPanel->Children.Num();
					if (LocalScrollAmount > 0)
					{
						for (int i = 0; i < MaxChilrenIndex; ++i)
						{
							SSphereCurBox::FSlot& ChildWidget = SphereCurPanel->Children[i];

							SizeDir -= WidgetIntervalPos;
							DesiredScrollOffset -= WidgetIntervalPos;

							SphereCurPanel->Children.Move(0, MaxChilrenIndex - 1);

							if (SizeDir <= 0)
							{

								SphereCurPanel->PhysicalOffset = DesiredScrollOffset;
								break;

							}
						}
					}

				}
			else if (DesiredScrollOffset + LocalScrollAmount < 0)
				{
					bAnimateScroll = false;
					float SizeDir = DesiredScrollOffset + LocalScrollAmount;
					if (LocalScrollAmount < 0)
					{
						int MaxChilrenIndex = SphereCurPanel->Children.Num();
						for (int i = 0; i < MaxChilrenIndex; ++i)
						{
							SSphereCurBox::FSlot& ChildWidget = SphereCurPanel->Children[MaxChilrenIndex - 1];

							SizeDir += WidgetIntervalPos;
							DesiredScrollOffset += WidgetIntervalPos;

							SphereCurPanel->Children.Move(MaxChilrenIndex - 1, 0);
							if (SizeDir >= 0)
							{

								SphereCurPanel->PhysicalOffset = DesiredScrollOffset;
								break;
							}
						}
					}
					
				}
			bAnimateScroll = InAnimateScroll;
			DesiredScrollOffset = DesiredScrollOffset + LocalScrollAmount;
			OnUserScrolled.ExecuteIfBound(DesiredScrollOffset);
			return ConsumeMouseWheel == EConsumeMouseWheel::Always || DesiredScrollOffset != PreviousScrollOffset;
		}
		else
		{
			bAnimateScroll = InAnimateScroll;
			DesiredScrollOffset = FMath::Clamp(DesiredScrollOffset + LocalScrollAmount, ScrollMin, ScrollMax);
			OnUserScrolled.ExecuteIfBound(DesiredScrollOffset);
		}

	}
	return ConsumeMouseWheel == EConsumeMouseWheel::Always || DesiredScrollOffset != PreviousScrollOffset;
}

EActiveTimerReturnType SSphereCurBox::UpdateInertialScroll(double InCurrentTime, float InDeltaTime)
{
	bool bKeepTicking = bIsScrolling;

	if (bIsScrolling)
	{
		InertialScrollManager.UpdateScrollVelocity(InDeltaTime);
		float ScrollVelocity = InertialScrollManager.GetScrollVelocity();
		const float ScrollVelocityLocal = InertialScrollManager.GetScrollVelocity() / CachedGeometry.Scale;
		//UE_LOG(LogTemp, Log, TEXT("%f"), ScrollVelocity);
		if (ScrollVelocityLocal != 0.f)
		{
			if (CanUseInertialScroll(ScrollVelocityLocal))
			{
				bKeepTicking = true;
				ScrollBy(CachedGeometry, ScrollVelocityLocal * InDeltaTime, AllowOverscroll, false);
				bInertialForwardMovement = ScrollVelocityLocal > 0;
			}
			else
			{
				InertialScrollManager.ClearScrollVelocity();
			}
		}
	}
	if (AllowOverscroll == EAllowOverscroll::Yes)
	{
		if (Overscroll.GetOverscroll(CachedGeometry) != 0.0f)
		{
			bKeepTicking = true;
		}

		Overscroll.UpdateOverscroll(InDeltaTime);
	}

	TickScrollDelta = 0.f;

	if (!bKeepTicking)
	{
		bIsScrolling = false;
		// 使得惯性结束对齐
		if (bAlignedWidget)
		{
			InertialOffsetOver(bInertialForwardMovement);
		}
		
		bIsScrollingActiveTimerRegistered = false;
		UpdateInertialScrollHandle.Reset();
	}

	return bKeepTicking ? EActiveTimerReturnType::Continue : EActiveTimerReturnType::Stop;
}


void SSphereCurPanel::Construct(const FArguments& InArgs, const TArray<SSphereCurBox::FSlot*>& InSlots)
{
	PhysicalOffset = 0;
	Children.Reserve(InSlots.Num());
	for (int32 SlotIndex = 0; SlotIndex < InSlots.Num(); ++SlotIndex)
	{
		Children.Add(InSlots[SlotIndex]);
	}
	SpherePosAlignment = InArgs._SpherePosAlignment;
	WidgetIntervalPos = InArgs._WidgetIntervalPos;
	SphereRadius = InArgs._SphereRadius;
	bLoopChildren = InArgs._bLoopChildren;
}

void SSphereCurPanel::SetSpherePosAlignment(ESpherePosAlignment InSpherePosAlignment)
{
	SpherePosAlignment = InSpherePosAlignment;
}

void SSphereCurPanel::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	float CurChildOffset = -PhysicalOffset;

	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		const SSphereCurBox::FSlot& ThisSlot = Children[SlotIndex];
		const EVisibility ChildVisibility = ThisSlot.GetWidget()->GetVisibility();

		if (ChildVisibility != EVisibility::Collapsed)
		{
			if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
			{
				CurChildOffset = ArrangeChildVerticalAndReturnOffset(AllottedGeometry, ArrangedChildren, ThisSlot, CurChildOffset);
			}
			else
			{
				CurChildOffset = ArrangeChildHorizontalAndReturnOffset(AllottedGeometry, ArrangedChildren, ThisSlot, CurChildOffset);
			}
		}
	}
}

FVector2D SSphereCurPanel::ComputeDesiredSize(float) const
{
	FVector2D ThisDesiredSize = FVector2D::ZeroVector;

	int EndShowWidgetID;

	for (int32 SlotIndex = 0; SlotIndex < Children.Num(); ++SlotIndex)
	{
		const SSphereCurBox::FSlot& ThisSlot = Children[SlotIndex];

		if (ThisSlot.GetWidget()->GetVisibility() != EVisibility::Collapsed)
		{
			const FVector2D ChildDesiredSize = ThisSlot.GetWidget()->GetDesiredSize() * (FVector2D(1.f,1.f) - this->ChildrenAlignment);
			switch (SpherePosAlignment)
			{
			case ESpherePosAlignment::SAlign_Top:
				ThisDesiredSize.Y = FMath::Max(ThisSlot.GetWidget()->GetCachedGeometry().Position.Y + ChildDesiredSize.Y + ThisSlot.SlotPadding.Get().Top, ThisDesiredSize.Y);
				ThisDesiredSize.X += WidgetIntervalPos;
				break;
			case ESpherePosAlignment::SAlign_Left:
				ThisDesiredSize.X = FMath::Max(ThisSlot.GetWidget()->GetCachedGeometry().Position.X + ChildDesiredSize.X + ThisSlot.SlotPadding.Get().Left, ThisDesiredSize.X);
				ThisDesiredSize.Y += WidgetIntervalPos;
				break;
			case ESpherePosAlignment::SAlign_Bottom:
				ThisDesiredSize.Y = FMath::Max(ThisSlot.GetWidget()->GetCachedGeometry().Position.Y + ChildDesiredSize.Y + ThisSlot.SlotPadding.Get().Bottom, ThisDesiredSize.Y);
				ThisDesiredSize.X += WidgetIntervalPos;
				break;
			case ESpherePosAlignment::SAlign_Right:
				ThisDesiredSize.X = FMath::Max(ThisSlot.GetWidget()->GetCachedGeometry().Position.X + ChildDesiredSize.X + ThisSlot.SlotPadding.Get().Right, ThisDesiredSize.X);
				ThisDesiredSize.Y += WidgetIntervalPos;
				break;
			}
			EndShowWidgetID = SlotIndex;
		}
	}
	/*
	const SSphereCurBox::FSlot& EndWidgetSlot = Children[EndShowWidgetID];

	if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
	{
		if (EndWidgetSlot.GetWidget()->GetDesiredSize().Y < WidgetIntervalPos)
		{
			float EndWidgetSlotDir = WidgetIntervalPos - EndWidgetSlot.GetWidget()->GetDesiredSize().Y;
			ThisDesiredSize -= FVector2D(0, EndWidgetSlotDir);
		}
	}
	else
	{
		if (EndWidgetSlot.GetWidget()->GetDesiredSize().X < WidgetIntervalPos)
		{
			float EndWidgetSlotDir = WidgetIntervalPos - EndWidgetSlot.GetWidget()->GetDesiredSize().Y;
			ThisDesiredSize -= FVector2D(EndWidgetSlotDir, 0);
		}
	}
	*/
	return ThisDesiredSize;
}

float SSphereCurPanel::ArrangeChildVerticalAndReturnOffset(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, const SSphereCurBox::FSlot& ThisSlot, float CurChildOffset) const
{
	const FMargin& ThisPadding = ThisSlot.SlotPadding.Get();
	const FVector2D& WidgetDesiredSize = ThisSlot.GetWidget()->GetDesiredSize();
	const float ThisSlotHeight = ThisPadding.GetTotalSpaceAlong<Orient_Vertical>();

	const FVector2D WidgetSizeAlignmet = WidgetDesiredSize * this->ChildrenAlignment;

	AlignmentArrangeResult XAlignmentResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ThisSlot, ThisPadding);

	FVector2D SphereOffice = FVector2D::ZeroVector;
	FVector2D WidgetXY = FVector2D::ZeroVector;

	FVector2D WidgetPadding = FVector2D::ZeroVector;
	if (this->SpherePosAlignment == ESpherePosAlignment::SAlign_Left)
	{
		// 转换到以圆点为圆心的坐标系
		SphereOffice = GetSpherePointPos<ESpherePosAlignment::SAlign_Left>(AllottedGeometry.GetLocalSize().Y / 2 - CurChildOffset, SphereRadius, AllottedGeometry.GetLocalSize(), WidgetIntervalPos);
		// 转换到控件自身的坐标系
		float WidgetY = AllottedGeometry.GetLocalSize().Y / 2 - SphereOffice.Y;
		float WidgetX = SphereRadius + SphereOffice.X;
		WidgetXY = FVector2D(WidgetX, WidgetY);
		WidgetPadding = FVector2D(ThisPadding.Left, ThisPadding.Top);
	}
	else
	{
		// 转换到以圆点为圆心的坐标系
		SphereOffice = GetSpherePointPos<ESpherePosAlignment::SAlign_Right>(AllottedGeometry.GetLocalSize().Y / 2 - CurChildOffset, SphereRadius, AllottedGeometry.GetLocalSize(), WidgetIntervalPos);
		// 转换到控件自身的坐标系
		float WidgetY = AllottedGeometry.GetLocalSize().Y / 2 - SphereOffice.Y;
		float WidgetX = AllottedGeometry.GetLocalSize().X - SphereRadius + SphereOffice.X;
		WidgetXY = FVector2D(WidgetX, WidgetY);
		WidgetPadding = FVector2D(- ThisPadding.Right, - ThisPadding.Bottom);
	}
	

	FVector2D ChildOffset = FVector2D(XAlignmentResult.Offset + WidgetXY.X, WidgetXY.Y ) - WidgetSizeAlignmet + WidgetPadding;

	ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), ChildOffset, FVector2D(XAlignmentResult.Size, WidgetDesiredSize.Y)));

	return CurChildOffset + WidgetIntervalPos;
}

float SSphereCurPanel::ArrangeChildHorizontalAndReturnOffset(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, const SSphereCurBox::FSlot& ThisSlot, float CurChildOffset) const
{
	const FMargin& ThisPadding = ThisSlot.SlotPadding.Get();
	const FVector2D& WidgetDesiredSize = ThisSlot.GetWidget()->GetDesiredSize();
	const float ThisSlotDesiredWidth = WidgetDesiredSize.X + ThisPadding.GetTotalSpaceAlong<Orient_Horizontal>();

	const FVector2D WidgetSizeAlignmet = WidgetDesiredSize * this->ChildrenAlignment;

	AlignmentArrangeResult YAlignmentResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ThisSlot, ThisPadding);

	FVector2D SphereOffice = FVector2D::ZeroVector;

	FVector2D WidgetXY = FVector2D::ZeroVector;

	FVector2D WidgetPadding = FVector2D::ZeroVector;

	if (this->SpherePosAlignment == ESpherePosAlignment::SAlign_Top)
	{
		SphereOffice = GetSpherePointPos<ESpherePosAlignment::SAlign_Top>(CurChildOffset - (AllottedGeometry.GetLocalSize().X / 2),SphereRadius, AllottedGeometry.GetLocalSize(), WidgetIntervalPos);
		// 转换到控件自身的坐标系
		float WidgetY = SphereRadius - SphereOffice.Y;
		float WidgetX = AllottedGeometry.GetLocalSize().X / 2 + SphereOffice.X;
		WidgetXY = FVector2D(WidgetX, WidgetY);
		WidgetPadding = FVector2D(ThisPadding.Left, ThisPadding.Top);
	}
	else
	{
		SphereOffice = GetSpherePointPos<ESpherePosAlignment::SAlign_Bottom>(CurChildOffset - (AllottedGeometry.GetLocalSize().X / 2), SphereRadius, AllottedGeometry.GetLocalSize(), WidgetIntervalPos);
		// 转换到控件自身的坐标系
		float WidgetY =  - SphereOffice.Y - SphereRadius + AllottedGeometry.GetLocalSize().Y;
		float WidgetX = AllottedGeometry.GetLocalSize().X / 2 + SphereOffice.X;
		WidgetXY = FVector2D(WidgetX, WidgetY);
		WidgetPadding = FVector2D( - ThisPadding.Right, - ThisPadding.Bottom);
	}
	FVector2D ChildOffset = FVector2D(CurChildOffset, YAlignmentResult.Offset + WidgetXY.Y) - WidgetSizeAlignmet + WidgetPadding;

	ArrangedChildren.AddWidget(AllottedGeometry.MakeChild(ThisSlot.GetWidget(), ChildOffset, FVector2D(WidgetDesiredSize.X, YAlignmentResult.Size)));
	return CurChildOffset + WidgetIntervalPos;
}
