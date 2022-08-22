(function (app) {
    'use strict';

    angular.module('nb').controller('nbBCtrl', nbBController);
    
    nbBController.$inject = ['$scope'];
    function nbBController($scope) {
        var self = this;
        var scope = $scope;

        scope.in_data = {};
        scope.in_data.firstName = "aa";
        scope.in_data.lastName = "bb";

        scope.getFullName = function() {
            return scope.in_data.firstName + " " + scope.in_data.lastName;
        }
    }

})(NetBrainApp);