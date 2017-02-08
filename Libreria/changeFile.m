function changeFile( fileName, path )

% This code was developed for the Aerospace Guidance and Control Course, held in Politecnico di Milano with courtesy of MathWorks (R).
% Author: Silvano "Redman" Seva
% Support: silvano.seva@skywareder.eu
% May 24 2015
% Version 1.0 - codename Juno

% this program opens the block's cpp wrapper file and substitutes the libraries
% paths with the absolute paths pointing to the
% folder that contains the libraries needed by the block

% NOTE: in the 'if' call below the code MUST have a single '&' (or '|') instead of
% a '&&' ( a '||'), otherwise it will not work!!!

inputFile = fopen(fileName,'r+');
outputFile = fopen('partial.tmp','w+');

while( ~feof(inputFile) )
    textline = fgetl(inputFile);
    
    if(strcmp(textline,'#include "Wire.h"') | strcmp(textline,'#include <Wire.h>'))  %here the program searchs for the line '#include "Wire.h"'
                                                                                    %and substitutes it with the new path to the libs
        fwrite(outputFile,['#include "',path,'\lib\Wire.h";',char(10)]);
        fwrite(outputFile,['#include "',path,'\lib\Wire.cpp";',char(10)]);
        fwrite(outputFile,['#include "',path,'\lib\twi.h";',char(10)]);
        fwrite(outputFile,['#include "',path,'\lib\twi.c";',char(10)]);
        
        disp('file changed!!');
    
    elseif(strfind(textline,'#include') & strfind(textline,'Wire.h'))        %if the string '#include "Wire.h"' is not found the program
           fwrite(outputFile,['#include "',path,'\lib\Wire.h";',char(10)]);  %tries to change the single libraries' path
           disp('wire.h path changed!!');
    
    elseif(strfind(textline,'#include') & strfind(textline,'Wire.cpp'))     %here the program MUST have a single '&', otherwise it will not work!!!
           fwrite(outputFile,['#include "',path,'\lib\Wire.cpp";',char(10)]);
           disp('wire.cpp path changed!!');
   
    elseif(strfind(textline,'#include') & strfind(textline,'twi.h'))
           fwrite(outputFile,['#include "',path,'\lib\twi.h";',char(10)]);
           disp('twi.h path changed!!');
    
    elseif(strfind(textline,'#include') & strfind(textline,'twi.c'))
           fwrite(outputFile,['#include "',path,'\lib\twi.c";',char(10)]);
           disp('twi.cpp changed!!');
    
    else
        fwrite(outputFile,[textline,char(10)]);               %nothing changed, write unmodifid string to the new file
    end
end


fclose(outputFile);
fclose(inputFile);

movefile('partial.tmp',fileName);


end

