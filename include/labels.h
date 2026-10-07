/* labels.h -- five small word lists, by the index the code reads them at.
 *
 * Source: list resources in COLTEXT0.DLL, one entry a line, each read by load_data into
 * its table in order, so the line index IS the index:
 *
 *   CMESSAGE (TEXT 135)  CMSG_*        g_colony_message_words, and colony_message(idx)
 *   CTITLE   (TEXT 153)  CTITLE_*      g_ctitle_word
 *   ROUTE    (TEXT 513)  ROUTE_LABEL_* g_route_label
 *   INFO     (TEXT 291)  INFO_LABEL_*  g_info_label
 *   OTHER_NAMES (TEXT 463)  FEATURE_*     g_feature_words
 *
 * tools/check_headers.py re-derives every value, with misc.h's naming rule: the text as an
 * identifier, cut so the whole macro stays within 31 characters; a repeated text takes its
 * occurrence number (INFO has "With:" twice: INFO_LABEL_WITH and INFO_LABEL_WITH_2).
 */
#ifndef LABELS_H
#define LABELS_H

/* CMESSAGE -> g_colony_message_words, and colony_message(idx) */
#define CMSG_BOUGHT_FOR                 0
#define CMSG_SOLD_FOR                   1
#define CMSG_MOVED_TO                   2
#define CMSG_PRICE                      3
#define CMSG_NO_ROOM_FOR                4
#define CMSG_NOTHING_TO_TRANSPORT       5
#define CMSG_NOT_ENOUGH                 6
#define CMSG_TO_FILL_CARGO_HOLD         7
#define CMSG_TOO_EXPENSIVE              8
#define CMSG_NO                         9
#define CMSG_IN_CARGO_HOLD              10
#define CMSG_NOTHING_TO_TRANSFER        11
#define CMSG_AT                         12
#define CMSG_BUYING                     13
#define CMSG_SELLING                    14
#define CMSG_LOADING                    15
#define CMSG_UNLOADING                  16
#define CMSG_TAX                        17
#define CMSG_NET                        18

/* CTITLE -> g_ctitle_word */
#define CTITLE_POP                      0
#define CTITLE_GOLD                     1
#define CTITLE_BUY                      2
#define CTITLE_CHANGE                   3
#define CTITLE_SELECT_AN_ITEM_TO_BUILD  4
#define CTITLE_NO_PRODUCTION            5
#define CTITLE_MORE                     6
#define CTITLE_TURNS                    7
#define CTITLE_SELECT_A_PROFESSION_FOR  8
#define CTITLE_TAX                      9

/* ROUTE -> g_route_label */
#define ROUTE_LABEL_EDIT_TRADE_ROUTE    0
#define ROUTE_LABEL_ROUTE_NAME          1
#define ROUTE_LABEL_ROUTE_TYPE          2
#define ROUTE_LABEL_SEA                 3
#define ROUTE_LABEL_LAND                4
#define ROUTE_LABEL_DESTINATION         5
#define ROUTE_LABEL_UNLOAD_CARGO        6
#define ROUTE_LABEL_LOAD_CARGO          7
#define ROUTE_LABEL_DELETE_DESTINATION  8

/* INFO -> g_info_label */
#define INFO_LABEL_MOVES                0
#define INFO_LABEL_LOCAT                1
#define INFO_LABEL_WITH                 2
#define INFO_LABEL_WITH_2               3

/* OTHER_NAMES -> g_feature_words */
#define FEATURE_FOREST                  0
#define FEATURE_RIVER                   1
#define FEATURE_MAJOR_RIVER             2
#define FEATURE_MINOR_RIVER             3
#define FEATURE_UNEXPLORED              4

#endif
