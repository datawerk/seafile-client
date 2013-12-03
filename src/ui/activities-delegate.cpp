#include "activities-delegate.h"
#include <QApplication>
#include <QPainter>

namespace {

const int kMarginLeft = 5;
const int kMarginRight = 5;
const int kMarginTop = 5;
const int kMarginBottom = 5;
const int kPadding = 5;

const int kAvatarHeight = 36;
const int kAvatarWidth = 36;
const int kAvatarDescWidth = 180;
const int kAvatarDescHeight = 30;

const int kMarginBetweenAvatarAndDesc = 5;


QString fitTextToWidth(const QString& text, const QFont& font, int width)
{
    static QString ELLIPSISES = "...";

    QFontMetrics qfm(font);
    QSize size = qfm.size(0, text);
    if (size.width() <= width)
        return text;				// it fits, so just display it

    // doesn't fit, so we need to truncate and add ellipses
    QSize sizeElippses = qfm.size(0, ELLIPSISES); // we need to cut short enough to add these
    QString s = text;
    while (s.length() > 20)     // never cut shorter than this...
    {
        int len = s.length();
        s = text.left(len-1);
        size = qfm.size(0, s);
        if (size.width() <= (width - sizeElippses.width()))
            break;              // we are finally short enough
    }

    return (s + ELLIPSISES);
}

QFont zoomFont(const QFont& font_in, double ratio)
{
    QFont font(font_in);

    if (font.pointSize() > 0) {
        font.setPointSize((int)(font.pointSize() * ratio));
    } else {
        font.setPixelSize((int)(font.pixelSize() * ratio));
    }

    return font;
}

}

ActivitiesDelegate::ActivitiesDelegate()
{
}

void ActivitiesDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyledItemDelegate::paint(painter,option,index);

    QBrush backBrush;
    QColor foreColor;
    bool hover = false;
    bool selected = false;
    if (option.state & (QStyle::State_HasFocus | QStyle::State_Selected)) {
        backBrush = option.palette.brush(QPalette::Highlight);
        backBrush = QColor("#f9e0c7");
        foreColor = option.palette.color( QPalette::Text );
        selected = true;

    } else if (option.state & QStyle::State_MouseOver) {

    } else {
        backBrush = option.palette.brush( QPalette::Base );
        foreColor = option.palette.color( QPalette::Text );
    }

    QStyleOptionViewItemV4 opt(option);
    if (hover)
    {
        Qt::BrushStyle bs = opt.backgroundBrush.style();
        if (bs > Qt::NoBrush && bs < Qt::TexturePattern)
            opt.backgroundBrush = opt.backgroundBrush.color().lighter(115);
        else
            opt.backgroundBrush = backBrush;
    }
    painter->save();
    painter->fillRect(option.rect, backBrush);
    painter->restore();

    // Paint avatar icon
    QPoint avatar_icon_pos(kMarginLeft + kPadding, kMarginTop + kPadding);
    avatar_icon_pos += option.rect.topLeft();
    QIcon icon = qvariant_cast<QIcon>(index.data(ICON_ROLE));
    painter->save();
    painter->drawPixmap(avatar_icon_pos,
                        icon.pixmap(36));
    painter->restore();

    // Paint events description
    painter->save();
    QPoint avatar_desc_pos = avatar_icon_pos + QPoint(kAvatarWidth + kMarginBetweenAvatarAndDesc, 0);
    QRect avatar_desc_rect(avatar_desc_pos, QSize(kAvatarDescWidth, kAvatarDescHeight));
    painter->setPen(foreColor);
    QString header_text = qvariant_cast<QString>(index.data(HEADER_TEXT_ROLE));
    painter->drawText(avatar_desc_rect,
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                      fitTextToWidth(header_text, option.font, kAvatarDescWidth),
                      &avatar_desc_rect);

    // Paint events time
    QPoint avatar_time_pos = avatar_desc_rect.bottomLeft() + QPoint(0, 5);
    QRect avatar_time_rect(avatar_time_pos, QSize(kAvatarDescWidth, kAvatarDescHeight));
    painter->setPen(selected ? foreColor : foreColor.lighter(150));
    painter->setFont(zoomFont(painter->font(), 0.8));

    QString sub_text = qvariant_cast<QString>(index.data(SUB_TEXT_ROLE));
    //description = translateCommitTime(repo.mtime);
    painter->drawText(avatar_time_rect,
                      Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap,
                      fitTextToWidth(sub_text, option.font, kAvatarDescWidth),
                      &avatar_time_rect);
    painter->restore();
}

QSize ActivitiesDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int width = kMarginLeft + kAvatarWidth
        + kMarginBetweenAvatarAndDesc + kAvatarDescWidth
        + kMarginRight + kPadding * 2;

    int height = kAvatarHeight + kPadding * 2 + kMarginTop + kMarginBottom;

    return QSize(width, height);
}
