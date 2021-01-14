//
//  Copyright (c) 2021 Open Whisper Systems. All rights reserved.
//

#import "BaseModel.h"

NS_ASSUME_NONNULL_BEGIN

BOOL IsNoteToSelfEnabled(void);

@class GRDBReadTransaction;
@class MessageBody;
@class MessageBodyRanges;
@class OWSDisappearingMessagesConfiguration;
@class SDSAnyReadTransaction;
@class SDSAnyWriteTransaction;
@class SignalServiceAddress;
@class TSInteraction;
@class TSInvalidIdentityKeyReceivingErrorMessage;

typedef NSString *ConversationColorName NS_STRING_ENUM;

extern ConversationColorName const ConversationColorNameCrimson;
extern ConversationColorName const ConversationColorNameVermilion;
extern ConversationColorName const ConversationColorNameBurlap;
extern ConversationColorName const ConversationColorNameForest;
extern ConversationColorName const ConversationColorNameWintergreen;
extern ConversationColorName const ConversationColorNameTeal;
extern ConversationColorName const ConversationColorNameBlue;
extern ConversationColorName const ConversationColorNameIndigo;
extern ConversationColorName const ConversationColorNameViolet;
extern ConversationColorName const ConversationColorNamePlum;
extern ConversationColorName const ConversationColorNameTaupe;
extern ConversationColorName const ConversationColorNameSteel;

extern ConversationColorName const ConversationColorNameDefault;

typedef NS_CLOSED_ENUM(NSUInteger, TSThreadMentionNotificationMode) { TSThreadMentionNotificationMode_Default = 0,
    TSThreadMentionNotificationMode_Always,
    TSThreadMentionNotificationMode_Never };

/**
 *  TSThread is the superclass of TSContactThread and TSGroupThread
 */
@interface TSThread : BaseModel

@property (nonatomic) BOOL shouldThreadBeVisible;
@property (nonatomic, readonly, nullable) NSDate *creationDate;
@property (nonatomic, readonly) BOOL isArchivedByLegacyTimestampForSorting DEPRECATED_MSG_ATTRIBUTE("this property is only to be used in the sortId migration");
@property (nonatomic, readonly) BOOL isArchived;
@property (nonatomic, readonly) BOOL isMarkedUnread;

// This maintains the row Id that was at the bottom of the conversation
// the last time the user viewed this thread so we can restore their
// scroll position.
//
// If the referenced message is deleted, this value is
// updated to point to the previous message in the conversation.
//
// If a new message is inserted into the conversation, this value
// is cleared. We only restore this state if there are no unread messages.
@property (nonatomic, readonly) uint64_t lastVisibleSortIdObsolete;
@property (nonatomic, readonly) double lastVisibleSortIdOnScreenPercentageObsolete;

// zero if thread has never had an interaction.
// The corresponding interaction may have been deleted.
@property (nonatomic, readonly) int64_t lastInteractionRowId;

- (instancetype)init NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithUniqueId:(NSString *)uniqueId NS_DESIGNATED_INITIALIZER;
- (instancetype)initWithGrdbId:(int64_t)grdbId uniqueId:(NSString *)uniqueId NS_UNAVAILABLE;

// --- CODE GENERATION MARKER

// This snippet is generated by /Scripts/sds_codegen/sds_generate.py. Do not manually edit it, instead run `sds_codegen.sh`.

// clang-format off

- (instancetype)initWithGrdbId:(int64_t)grdbId
                      uniqueId:(NSString *)uniqueId
           conversationColorName:(ConversationColorName)conversationColorName
                    creationDate:(nullable NSDate *)creationDate
                      isArchived:(BOOL)isArchived
                  isMarkedUnread:(BOOL)isMarkedUnread
            lastInteractionRowId:(int64_t)lastInteractionRowId
       lastVisibleSortIdObsolete:(uint64_t)lastVisibleSortIdObsolete
lastVisibleSortIdOnScreenPercentageObsolete:(double)lastVisibleSortIdOnScreenPercentageObsolete
         mentionNotificationMode:(TSThreadMentionNotificationMode)mentionNotificationMode
                    messageDraft:(nullable NSString *)messageDraft
          messageDraftBodyRanges:(nullable MessageBodyRanges *)messageDraftBodyRanges
                  mutedUntilDate:(nullable NSDate *)mutedUntilDate
           shouldThreadBeVisible:(BOOL)shouldThreadBeVisible
NS_DESIGNATED_INITIALIZER NS_SWIFT_NAME(init(grdbId:uniqueId:conversationColorName:creationDate:isArchived:isMarkedUnread:lastInteractionRowId:lastVisibleSortIdObsolete:lastVisibleSortIdOnScreenPercentageObsolete:mentionNotificationMode:messageDraft:messageDraftBodyRanges:mutedUntilDate:shouldThreadBeVisible:));

// clang-format on

// --- CODE GENERATION MARKER

@property (nonatomic) ConversationColorName conversationColorName;

- (void)updateConversationColorName:(ConversationColorName)colorName transaction:(SDSAnyWriteTransaction *)transaction;
+ (ConversationColorName)stableColorNameForNewConversationWithString:(NSString *)colorSeed;
@property (class, nonatomic, readonly) NSArray<ConversationColorName> *conversationColorNames;

/**
 * @returns recipientId for each recipient in the thread
 */
@property (nonatomic, readonly) NSArray<SignalServiceAddress *> *recipientAddresses;

@property (nonatomic, readonly) BOOL isNoteToSelf;

#pragma mark Interactions

/**
 *  @return The number of interactions in this thread.
 */
- (NSUInteger)numberOfInteractionsWithTransaction:(SDSAnyReadTransaction *)transaction;

/**
 * Get all messages in the thread we weren't able to decrypt
 */
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
- (NSArray<TSInvalidIdentityKeyReceivingErrorMessage *> *)receivedMessagesForInvalidKey:(NSData *)key;
#pragma clang diagnostic pop

- (BOOL)hasSafetyNumbers;

- (void)markAllAsReadAndUpdateStorageService:(BOOL)updateStorageService
                                 transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(markAllAsRead(updateStorageService:transaction:));

- (void)markAsUnreadAndUpdateStorageService:(BOOL)updateStorageService
                                transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(markAsUnread(updateStorageService:transaction:));
- (void)clearMarkedAsUnreadAndUpdateStorageService:(BOOL)updateStorageService
                                       transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(clearMarkedAsUnread(updateStorageService:transaction:));

- (nullable TSInteraction *)lastInteractionForInboxWithTransaction:(SDSAnyReadTransaction *)transaction
    NS_SWIFT_NAME(lastInteractionForInbox(transaction:));

- (nullable TSInteraction *)firstInteractionAtOrAroundSortId:(uint64_t)sortId
                                                 transaction:(SDSAnyReadTransaction *)transaction
    NS_SWIFT_NAME(firstInteraction(atOrAroundSortId:transaction:));

/**
 *  Updates the thread's caches of the latest interaction.
 *
 *  @param message Latest Interaction to take into consideration.
 *  @param transaction Database transaction.
 */
- (void)updateWithInsertedMessage:(TSInteraction *)message transaction:(SDSAnyWriteTransaction *)transaction;
- (void)updateWithUpdatedMessage:(TSInteraction *)message transaction:(SDSAnyWriteTransaction *)transaction;
- (void)updateWithRemovedMessage:(TSInteraction *)message transaction:(SDSAnyWriteTransaction *)transaction;
- (BOOL)hasPendingMessageRequestWithTransaction:(GRDBReadTransaction *)transaction
    NS_SWIFT_NAME(hasPendingMessageRequest(transaction:));

#pragma mark Archival

/**
 *  Archives a thread
 *
 *  @param transaction Database transaction.
 */
- (void)archiveThreadAndUpdateStorageService:(BOOL)updateStorageService
                                 transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(archiveThread(updateStorageService:transaction:));

- (void)softDeleteThreadWithTransaction:(SDSAnyWriteTransaction *)transaction;

/**
 *  Unarchives a thread
 *
 *  @param transaction Database transaction.
 */
- (void)unarchiveThreadAndUpdateStorageService:(BOOL)updateStorageService
                                   transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(unarchiveThread(updateStorageService:transaction:));

- (void)unarchiveAndMarkVisibleThreadWithUpdateStorageService:(BOOL)updateStorageService
                                                  transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(unarchiveThreadAndMarkVisible(updateStorageService:transaction:));

- (void)removeAllThreadInteractionsWithTransaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(removeAllThreadInteractions(transaction:));


#pragma mark Disappearing Messages

- (OWSDisappearingMessagesConfiguration *)disappearingMessagesConfigurationWithTransaction:
    (SDSAnyReadTransaction *)transaction;
- (uint32_t)disappearingMessagesDurationWithTransaction:(SDSAnyReadTransaction *)transaction;

#pragma mark Drafts

/**
 *  Returns the last known draft for that thread. Always returns a string. Empty string if nil.
 *
 *  @param transaction Database transaction.
 *
 *  @return Last known draft for that thread.
 */
- (nullable MessageBody *)currentDraftWithTransaction:(SDSAnyReadTransaction *)transaction;

/**
 *  Sets the draft of a thread. Typically called when leaving a conversation view.
 *
 *  @param draftMessageBody Draft to be saved.
 *  @param transaction Database transaction.
 */
- (void)updateWithDraft:(nullable MessageBody *)draftMessageBody transaction:(SDSAnyWriteTransaction *)transaction;

@property (atomic, readonly) BOOL isMuted;
@property (atomic, readonly, nullable) NSDate *mutedUntilDate;

@property (nonatomic, readonly) TSThreadMentionNotificationMode mentionNotificationMode;

#pragma mark - Update With... Methods

- (void)updateWithMutedUntilDate:(nullable NSDate *)mutedUntilDate transaction:(SDSAnyWriteTransaction *)transaction;

- (void)updateWithMentionNotificationMode:(TSThreadMentionNotificationMode)mentionNotificationMode
                              transaction:(SDSAnyWriteTransaction *)transaction
    NS_SWIFT_NAME(updateWithMentionNotificationMode(_:transaction:));

+ (BOOL)shouldInteractionAppearInInbox:(TSInteraction *)interaction
                           transaction:(SDSAnyReadTransaction *)transaction
    NS_SWIFT_NAME(shouldInteractionAppearInInbox(_:transaction:));

@end

NS_ASSUME_NONNULL_END
