/////////////////////////////////////////////////////////////////////////////////////////
// 
//                 Université d’Aix Marseille (AMU) - 
//                 Institut National de la Santé et de la Recherche Médicale (INSERM)
//                 Copyright © 2013 AMU, INSERM
// 
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 3 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
//
//    Author: Bruno Colombet – Laboratoire UMR INS INSERM 1106 - Bruno.Colombet@univ-amu.fr
//
//////////////////////////////////////////////////////////////////////////////////////////
#include "common.h"
#include <AwProcess.h>

static mxArray *parse_cfg(const mxArray *);

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if (nrhs > 1) {
		// one parameter only is required.
		mexErrMsgTxt("Wrong number of parameters (See help).");
		return;
	}
	
	mxArray *out = NULL, *in = NULL;

	if (nrhs == 1) {
		if (mxGetClassID(prhs[0]) != mxSTRUCT_CLASS) {
			mexErrMsgTxt("the parameter must be a struct (See help).");
			return;
		}
		out = parse_cfg(prhs[0]);
	}

	if (nlhs == 0) {
		out = parse_cfg(0);
	}

	if (out)
		plhs[0] = out;
}

/// parse the cfg structure set as input parameter
mxArray *parse_cfg(const mxArray *cfg)
{
	TCPRequest request(AwRequest::GetDataEx);
	if (request.status() != TCPRequest::connected)
		return NULL;

	QByteArray data;

	// if cfg == NULL => send an empty string (default parameters = all data from current input channels set for the process).
	if (cfg == NULL) {
		data.append(QString());
	}
	else {
		data.append(toJson(cfg));

	}
	if (!request.sendRequest(data))
		return NULL;


	


}

