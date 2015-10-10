var request = require('request');

var SOURCE_CODE_GET_CONSTANTS = {
    GITHUB: 'github',
    GITHUB_ROOT: 'https://raw.githubusercontent.com',
    VERSION: '1.0',
    TYPE: 'SOURCE_CODE_GET'
};

var GitHubSourceRetriever = {
    generateGithubSourceUrl: function(relativePath) {
        return SOURCE_CODE_GET_CONSTANTS.GITHUB_ROOT + decodeURIComponent(relativePath).replace("blob/","");
    },

    retrieveAsync: function(relativePath, successCallback, failureCallback) {
        if (relativePath === undefined || relativePath === null || relativePath === "") {
            failureCallback(new Error('ERROR: not valid source path'));
            return;
        }

        var url = GitHubSourceRetriever.generateGithubSourceUrl(relativePath);
        request(url, function (error, response, body) {
            if (!error && response.statusCode === 200) {
                successCallback(body);
            }
            else {
                failureCallback("ERROR: Distant server failed with " + JSON.stringify(response));
            }
        });
    }
};

var SourceRetriever = {
    isSupportedDataSource: function(dataSourceName) {
        if (dataSourceName === undefined || dataSourceName === null || dataSourceName === "") {
            return false;
        }

        return dataSourceName === SOURCE_CODE_GET_CONSTANTS.GITHUB;
    },

    retrieveAsync: function(dataSourceName, relativePath, successCallback, failureCallback) {
        GitHubSourceRetriever.retrieveAsync(dataSourceName, relativePath, successCallback, failureCallback);
    }
};

exports.handler = function(event, context) {
    console.log('INFOTRACE: Received event:', JSON.stringify(event, null, 2));

    if (!SourceRetriever.isSupportedDataSource(event.dataSourceName)) {
        context.fail(new Error('ERROR: dataSourceName is not valid'));
    }

    var successCallback = function(content) {
        var responseObject = {
            version: SOURCE_CODE_GET_CONSTANTS.VERSION,
            type: SOURCE_CODE_GET_CONSTANTS.TYPE,
            data_source_name: event.dataSourceName,
            relative_path: event.relativePath,
            source: content
        };
        context.succeed(responseObject);
    };

    var failureCallback = function(error) {
        context.fail(error);
    };

    SourceRetriever.retrieveAsync(event.relativePath, successCallback, failureCallback);
};