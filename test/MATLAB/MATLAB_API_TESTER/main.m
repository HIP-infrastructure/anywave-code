function main()
% testing AwSendMessage
AwSendMessage('AwSendMessage: sending a message...');
% getpluginio
test_getpluginio();
% getmarkers
test_getmarkers();
% getmarkers_ex
test_getmarkers_ex();
% getdata
test_getdata();
end