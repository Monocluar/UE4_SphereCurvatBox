// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Layout/Visibility.h"
#include "SlotBase.h"
#include "Styling/SlateColor.h"
#include "Input/CursorReply.h"
#include "Input/Reply.h"
#include "Input/NavigationReply.h"
#include "Widgets/SWidget.h"
#include "Widgets/SPanel.h"
#include "Layout/Children.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"
#include "Styling/CoreStyle.h"
#include "Widgets/Layout/SScrollBar.h"
#include "Framework/Layout/InertialScrollManager.h"
#include "Framework/Layout/Overscroll.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FOnAlignedScrolled, TSharedPtr<SWidget>);

class FPaintArgs;
class FSlateWindowElementList;
class SSphereCurPanel;

UENUM(BlueprintType)
enum class ESpherePosAlignment : uint8
{
	/** 依照控件上边生成 */
	SAlign_Top UMETA(DisplayName = "Top"),

	/** 依照控件左边生成 */
	SAlign_Left UMETA(DisplayName = "Left"),

	/** 依照控件下边生成 */
	SAlign_Bottom UMETA(DisplayName = "Bottom"),

	/** 依照控件右边生成 */
	SAlign_Right UMETA(DisplayName = "Right"),
};

class SPHERECURVATBOX_API SSphereCurBox : public SCompoundWidget
{
public:
	class FSlot : public TSlotBase<FSlot>, public TSupportsContentPaddingMixin<FSlot>
	{
	public:
		FSlot()
			: TSlotBase<FSlot>()
			// Set both vertical and horizontal alignment to fill by default.  During layout, the
			// alignment direction parallel to the scroll direction is assumed to be top, or left
			// since that is how the items are stacked.
			, HAlignment(HAlign_Left)
			, VAlignment(VAlign_Top)
		{
		}

		FSlot& HAlign(EHorizontalAlignment InHAlignment)
		{
			HAlignment = InHAlignment;
			return *this;
		}

		FSlot& VAlign(EVerticalAlignment InVAlignment)
		{
			VAlignment = InVAlignment;
			return *this;
		}

		EHorizontalAlignment HAlignment;
		EVerticalAlignment VAlignment;
	};

	SLATE_BEGIN_ARGS(SSphereCurBox)
		: _Style(&FCoreStyle::Get().GetWidgetStyle<FScrollBoxStyle>("ScrollBox"))
		, _ScrollBarStyle(&FCoreStyle::Get().GetWidgetStyle<FScrollBarStyle>("ScrollBar"))
		, _ExternalScrollbar()
		, _ScrollBarVisibility(EVisibility::Visible)
		, _ScrollBarDragFocusCause(EFocusCause::Mouse)
		, _ScrollBarAlwaysVisible(false)
		, _bLoopChildren(false)
		, _MonuseScrollKey(EKeys::RightMouseButton)
		, _ScrollBarThickness(FVector2D(5, 5))
		, _ShadowFadeDistance(10.f)
		, _SpherePosAlignment(ESpherePosAlignment::SAlign_Left)
		, _WidgetIntervalPos(10.f)
		, _SphereRadius(2000.f)
		, _bAlignedWidget(false)
		, _AllowOverscroll(EAllowOverscroll::Yes)
		, _ChildrenAlignment(FVector2D(0.f, 0.f))
		, _OnUserScrolled()
		, _OnAlignedScrolled()
		, _OnCenterAlignedScrolled()
		, _ConsumeMouseWheel(EConsumeMouseWheel::WhenScrollingPossible)
		{
			_Clipping = EWidgetClipping::ClipToBounds;
		}

		SLATE_SUPPORTS_SLOT(FSlot)

		// 自身下拉条风格
		SLATE_STYLE_ARGUMENT(FScrollBoxStyle, Style)

		// 拖动条风格
		SLATE_STYLE_ARGUMENT(FScrollBarStyle, ScrollBarStyle)

		// 拖动条指针
		SLATE_ARGUMENT(TSharedPtr<SScrollBar>, ExternalScrollbar)

		// 是否显示拖动条
		SLATE_ARGUMENT(EVisibility, ScrollBarVisibility)

		SLATE_ARGUMENT(EFocusCause, ScrollBarDragFocusCause)

		SLATE_ARGUMENT(bool, ScrollBarAlwaysVisible)

		SLATE_ARGUMENT(bool, bLoopChildren)

		SLATE_ARGUMENT(FKey, MonuseScrollKey)

		// 拖动条大小设置
		SLATE_ARGUMENT(FVector2D, ScrollBarThickness)

		// 滚动差值
		SLATE_ARGUMENT(float, ShadowFadeDistance)

		// 控件折叠方向
		SLATE_ARGUMENT(ESpherePosAlignment, SpherePosAlignment)

		// 控件间隔距离
		SLATE_ARGUMENT(float, WidgetIntervalPos)

		// 圆的半径
		SLATE_ARGUMENT(float, SphereRadius)

		SLATE_ARGUMENT(bool,bAlignedWidget)

		// 过渡动画
		SLATE_ARGUMENT(EAllowOverscroll, AllowOverscroll)

		// 子控件整体重心位置
		SLATE_ARGUMENT(FVector2D, ChildrenAlignment)

		// 绑定下拉条函数
		SLATE_EVENT(FOnUserScrolled, OnUserScrolled)

		// 惯性滑动最终跳转中心时传入中心子控件指针
		SLATE_EVENT(FOnAlignedScrolled, OnAlignedScrolled)

		// 每当一个控件在中心时传出中心子控件指针
		SLATE_EVENT(FOnAlignedScrolled, OnCenterAlignedScrolled)

		SLATE_ARGUMENT(EConsumeMouseWheel, ConsumeMouseWheel);

	SLATE_END_ARGS()

	SSphereCurBox();

	static FSlot& Slot();

	SSphereCurBox::FSlot& AddSlot();

	/**
	 * Construct this widget
	 *
	 * @param	InArgs	The declaration data for this widget
	 */
	void Construct(const FArguments& InArgs);

	void RemoveSlot(const TSharedRef<SWidget>& WidgetToRemove);

	void ClearChildren();

	bool IsRightClickScrolling() const;

	float GetScrollOffset() const;
	void SetScrollOffset(float NewScrollOffset);

	float GetViewFraction() const;

	float GetViewOffsetFraction() const;

	/**
	 *尝试将小部件滚动到视图中，将安全地处理非后代小部件
	 *
	 * @param WidgetToFind我们希望发现其几何形状的小部件。
	 * @param InAnimateScroll是否为滚动动画
	 * @param InDestination我们希望子小部件停止的位置。
	 */
	void ScrollDescendantIntoView(const TSharedPtr<SWidget>& WidgetToFind, bool InAnimateScroll = true, float Padding = 0);

	/**
	 *尝试将小部件滚动到视图中，将安全地处理非后代小部件
	 *
	 * @param WidgetToFind我们希望发现其几何形状的小部件。
	 * @param IntoWidget跳转到的Widget
	 * @param InAnimateScroll是否为滚动动画
	 * @param InDestination我们希望子小部件停止的位置。
	 */
	void ScrollDescendantIntoWidget(const TSharedPtr<SWidget>& WidgetToFind, const TSharedPtr<SWidget>& IntoWidget, bool InAnimateScroll = true);

	void ScrollDescendantIntoOffset(float NewOffset, bool InAnimateScroll = true);

	void ScrollToStart();

	void ScrollToEnd();

	void SetSpherePosAlignment(ESpherePosAlignment InSpherePosAlignment);

	void SetScrollBarVisibility(EVisibility InVisibility);

	void SetScrollBarThickness(FVector2D InThickness);

	void SetScrollBarAlwaysVisible(bool InAlwaysVisible);

	void SetChildrenAlignment(FVector2D InChildrenAlignment);

	void SetAllowOverscroll(EAllowOverscroll InAllowOverscroll);

public:
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;
	virtual bool ComputeVolatility() const override;
	virtual FReply OnPreviewMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseWheel(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply OnTouchEnded(const FGeometry& MyGeometry, const FPointerEvent& InTouchEvent) override;
	virtual void OnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual FNavigationReply OnNavigation(const FGeometry& MyGeometry, const FNavigationEvent& InNavigationEvent) override;

private:
	/** 构建默认的滚动条 */
	TSharedPtr<SScrollBar> ConstructScrollBar();

	/** Constructs internal layout widgets for scrolling vertically using the existing ScrollPanel and ScrollBar. */
	void ConstructVerticalLayout();

	/** Constructs internal layout widgets for scrolling horizontally using the existing ScrollPanel and ScrollBar. */
	void ConstructHorizontalLayout();

	FORCEINLINE float GetScrollComponentFromVector(FVector2D Vector) const
	{
		return SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right ? Vector.Y : Vector.X;
	}

	inline void SetScrollComponentOnVector(FVector2D& InVector, float Value) const
	{
		if (SpherePosAlignment == ESpherePosAlignment::SAlign_Left || SpherePosAlignment == ESpherePosAlignment::SAlign_Right)
		{
			InVector.Y = Value;
		}
		else
		{
			InVector.X = Value;
		}
	}

	/* 用户要求的滚动偏移量。 在实际滚动到那里之前，我们将对其进行夹紧。 */
	float DesiredScrollOffset;

	void SphereCurBox_OnUserScrolled(float InScrollOffsetFraction);

	FSlateColor GetStartShadowOpacity() const;

	FSlateColor GetEndShadowOpacity() const;

	bool InternalScrollDescendantIntoView(const FGeometry& MyGeometry, const TSharedPtr<SWidget>& WidgetToFind, bool InAnimateScroll = true, float Padding = 0);

	bool InternalScrollDescendantIntoWidget(const FGeometry& MyGeometry, const TSharedPtr<SWidget>& WidgetToFind, const TSharedPtr<SWidget>& IntoWidget, bool InAnimateScroll = true);

	bool InternalScrollDescendantIntoOffice(const FGeometry& MyGeometry, float NewOffset, bool InAnimateScroll = true);

	bool ScrollBy(const FGeometry& AllottedGeometry, float LocalScrollAmount, EAllowOverscroll Overscroll, bool InAnimateScroll);

	// 起始惯性滚动
	EActiveTimerReturnType UpdateInertialScroll(double InCurrentTime, float InDeltaTime);

	// 检测是否允许惯性滚动
	bool CanUseInertialScroll(float ScrollAmount) const;

	// 惯性转动结束
	void InertialOffsetOver(bool IsForwardMovement);

	// 判定居中的控件
	void FindChildCenterWidget();

	FVector2D GetChildWidgetSpherePointPos(const SSphereCurBox::FSlot& Child);

	int GetPanelChildIndex(const TSharedPtr<SWidget>& WidgetToFind);

	void BeginInertialScrolling();

	void EndInertialScrolling();

	TSharedPtr<SWidget> GetKeyboardFocusableWidget(TSharedPtr<SWidget> InWidget);

protected:

	TSharedPtr<class SSphereCurPanel> SphereCurPanel;
	TSharedPtr<SScrollBar> ScrollBar;

	float AmountScrolledWhileRightMouseDown;

	ESpherePosAlignment SpherePosAlignment;
	FVector2D ChildrenAlignment;

	// 圆的半径
	float SphereRadius;

	// 是否滑动对齐
	bool bAlignedWidget : 1;

	// 控件间隔距离
	float WidgetIntervalPos;

	const FScrollBoxStyle* Style;

	const FScrollBarStyle* ScrollBarStyle;

	FOnUserScrolled OnUserScrolled;

	FOnAlignedScrolled OnAlignedScrolled;

	FOnAlignedScrolled OnCenterAlignedScrolled;

	EConsumeMouseWheel ConsumeMouseWheel;

	FGeometry CachedGeometry;

	TFunction<void(FGeometry)> ScrollIntoViewRequest;

	TSharedPtr<FActiveTimerHandle> UpdateInertialScrollHandle;

	// 每次滚动的值
	float TickScrollDelta;

	// 光标位置
	FVector2D SoftwareCursorPosition;

	/** 让对象以惯性滚动 */
	FInertialScrollManager InertialScrollManager;

	/** 是否有过渡动画 */
	EAllowOverscroll AllowOverscroll;

	/** 滚动状态管理 */
	FOverscroll Overscroll;

	/** 是否在滚动 */
	bool bIsScrolling : 1;

	/** 是否设置滚动动画 */
	bool bAnimateScroll : 1;

	// 是否显示下拉框
	bool bScrollBarIsExternal : 1;

	// 是否循环
	bool bLoopChildren : 1;

	/** 是否注册滚动动画定时器 */
	bool bIsScrollingActiveTimerRegistered : 1;

	// 是否下拉到最底部
	bool bScrollToEnd : 1;

	bool bTouchPanningCapture : 1;

	bool bAllowsRightClickDragScrolling : 1;

	bool bShowSoftwareCursor : 1;

	double LastScrollTime;

	TOptional<int32> bFingerOwningTouchInteraction;

	float PendingScrollTriggerAmount;

	// 滚动差值
	float ShadowFadeDistance;

	// 惯性滚动是否为正向
	bool bInertialForwardMovement;

	// 滚动速度
	float ShadowSpeed;

	// 鼠标滑动的值
	FKey MonuseScrollKey;

};

class SSphereCurPanel : public SPanel
{
public:
	SLATE_BEGIN_ARGS(SSphereCurPanel)
	{
		_Visibility = EVisibility::SelfHitTestInvisible;
	}


	// 控件折叠方向
	SLATE_ARGUMENT(ESpherePosAlignment, SpherePosAlignment)

	// 子控件整体重心位置
	SLATE_ARGUMENT(FVector2D, ChildrenAlignment)

	// 控件间隔距离
	SLATE_ARGUMENT(float, WidgetIntervalPos)

	// 圆的半径
	SLATE_ARGUMENT(float, SphereRadius)

	SLATE_ARGUMENT(bool, bLoopChildren)

		SLATE_END_ARGS()

		SSphereCurPanel()
		: Children(this)
	{
	}

	void Construct(const FArguments& InArgs, const TArray<SSphereCurBox::FSlot*>& InSlots);

public:

	void SetSpherePosAlignment(ESpherePosAlignment InSpherePosAlignment);

	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;

	virtual FChildren* GetChildren() override
	{
		return &Children;
	}

	float PhysicalOffset;

	TPanelChildren<SSphereCurBox::FSlot> Children;


	void SetChildrenAlignment(FVector2D InChildrenAlignment)
	{
		ChildrenAlignment = InChildrenAlignment;
	}

	bool GetLoopChildren()
	{
		return bLoopChildren;
	}

	void SetLoopChildren(bool InValue)
	{
		bLoopChildren = InValue;
	}

protected:
	virtual FVector2D ComputeDesiredSize(float) const override;

private:

	float ArrangeChildVerticalAndReturnOffset(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, const SSphereCurBox::FSlot& ThisSlot, float CurChildOffset) const;
	float ArrangeChildHorizontalAndReturnOffset(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren, const SSphereCurBox::FSlot& ThisSlot, float CurChildOffset) const;

private:

	ESpherePosAlignment SpherePosAlignment;
	FVector2D ChildrenAlignment;

	bool bLoopChildren : 1;

	// 视口中显示几个控件
	float WidgetIntervalPos;
	// 圆的半径
	float SphereRadius;
};
