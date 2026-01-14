-module(simple_logger).

-export([set_level/1, error/1, warning/1, info/1, debug/1]).

%
-define(ERROR, 1).
-define(WARNING, 2).
-define(INFO, 3).
-define(DEBUG, 4).

%
-spec set_level(Level :: integer()) -> ok.
set_level(Level) ->
    persistent_term:put(log_level, Level),
    ok.

%
-spec error(Message :: string()) -> ok.
error(Message) ->
    Level = persistent_term:get(log_level, ?INFO),
    case Level >= ?ERROR of
        true ->
            io:format(standard_error, "~s~n", [Message]);  % For errors
            % Comments should be ignored, also as lines in between
        false ->
            ok
    end.

%
-spec warning(Message :: string()) -> ok.
warning(Message) ->
    Level = persistent_term:get(log_level, ?INFO),
    case Level >= ?WARNING of
        true ->
            io:format(standard_error, "~s~n", [Message]);  % For warnings
        false ->
            ok
    end.

%
-spec info(Message :: string()) -> ok.
info(Message) ->
    Level = persistent_term:get(log_level, ?INFO),
    case Level >= ?INFO of
        true ->
            io:format(standard_error, "~s~n", [Message]);  % For info
        false ->
            ok
    end.

%
-spec debug(Message :: string()) -> ok.
debug(Message) ->
    Level = persistent_term:get(log_level, ?INFO),
    case Level >= ?DEBUG of
        true ->
            io:format(standard_error, "~s~n", [Message]);  % For debug
        false ->
            ok
    end.
