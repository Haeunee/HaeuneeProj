USE [Chat_DB]

SET ANSI_NULLS ON
GO
SET QUOTED_IDENTIFIER ON
GO
CREATE TABLE [dbo].[ChatLog](
	[LogType] [int] NOT NULL,
	[SocketNum] [int] NOT NULL,
	[LogStr] [nvarchar](512) NOT NULL,
	[Time] [datetime] NOT NULL
) ON [PRIMARY]
GO 
CREATE NONCLUSTERED INDEX [IX_T_Chat_Log_Time] ON [dbo].[ChatLog]
(
	[Time] ASC
)WITH (PAD_INDEX = OFF, STATISTICS_NORECOMPUTE = OFF, SORT_IN_TEMPDB = OFF, DROP_EXISTING = OFF, ONLINE = OFF, ALLOW_ROW_LOCKS = ON, ALLOW_PAGE_LOCKS = ON) ON [PRIMARY]
GO
-- �α׸� �߰��ϴ� ���ν���
-- EXEC [dbo].[Add_Chat_Log] @logType, @sock, @str
CREATE PROCEDURE [dbo].[Add_Chat_Log]
	@logType int = 0,
	@sock int = 0,
	@str nvarchar(512) = 0
AS
BEGIN
	SET NOCOUNT ON;
	INSERT INTO [dbo].[ChatLog] ([LogType], [SocketNum], [LogStr], [Time]) VALUES(@logType, @sock, @str, GETDATE())

END
RETURN 0
GO
-- ������ �α׸� �����ϴ� ���ν���
-- EXEC [dbo].[DelOldLogDataByDate] @out_Result OUTPUT, @DelQuery OUTPUT, @TableName, @ChkDate
CREATE PROCEDURE [dbo].[DelOldLogDataByDate]
	@out_Result BIGINT = 0 OUTPUT,
	@DelQuery NVARCHAR(1024) = 0 OUTPUT,
	@TableName NVARCHAR(256) = 0,
	@ChkDate NVARCHAR(256) = 0
AS
BEGIN
	SET NOCOUNT ON	-- ���� SELECT �� �ʱ�ȭ �ϴ� ����
	DECLARE @retVal AS BIGINT = 0;	-- ��ȯ �Ǵ� �����
	
	SET @DelQuery = 'DELETE FROM ' + @TableName + ' WHERE DATEDIFF(DD, CONVERT(DATETIME, ''' + @ChkDate +'''), [Time]) <= 0'
	EXEC(@DelQuery)

END