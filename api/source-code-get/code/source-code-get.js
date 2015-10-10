var request = require('request');

var SOURCE_CODE_GET_CONSTANTS = {
    GITHUB: 'github',
    GITHUB_ROOT: 'https://raw.githubusercontent.com'
};

var GitHubSourceRetriever = {
    generateGithubSourceUrl: function(relativePath) {
        return SOURCE_CODE_GET_CONSTANTS.GITHUB_ROOT + decodeURIComponent(relativePath).replace("blob/","");
    },

    retrieveAsync: function(relativePath, successCallback, failureCallback) {
        if (relativePath === undefined || relativePath === null || relativePath === "") {
            failureCallback(new Error('not valid source path'));
            return;
        }

        var url = GitHubSourceRetriever.generateGithubSourceUrl(relativePath);
        request(url, function (error, response, body) {
            if (!error && response.statusCode === 200) {
                successCallback(body);
            }
            else {
                failureCallback("Distant server failed with " + JSON.stringify(response));
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
    console.log('Received event:', JSON.stringify(event, null, 2));

    if (!SourceRetriever.isSupportedDataSource(event.dataSourceName)) {
        context.fail(new Error('dataSourceName is not valid'));
    }

    var successCallback = function(content) {
        context.succeed(content);
    };

    var failureCallback = function(error) {
        context.fail(error);
    };

    SourceRetriever.retrieveAsync(event.relativePath, successCallback, failureCallback);
};